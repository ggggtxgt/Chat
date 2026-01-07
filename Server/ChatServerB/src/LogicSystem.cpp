#include "Glog.h"
#include "Const.h"
#include "UserManager.h"
#include "LogicSystem.h"
#include "MysqlManager.h"
#include "RedisManager.h"
#include "ChatGrpcClient.h"
#include "StatusGrpcClient.h"

LogicSystem::LogicSystem() : _b_stop(false) {
    RegisterCallBacks();
    _worker_thread = std::thread(&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem() {
    _b_stop = true;
    _consume.notify_one();
    _worker_thread.join();
}

void LogicSystem::PostMsgToQueue(std::shared_ptr<LogicNode> msg) {
    std::unique_lock<std::mutex> lock(_mutex);
    _msg_que.push(msg);
    // 由0变成1则发送通知信号
    if (1 == _msg_que.size()) {
        lock.unlock();
        _consume.notify_one();
    }
}

void LogicSystem::DealMsg() {
    for (;;) {
        std::unique_lock<std::mutex> lock(_mutex);
        // 判断队列是为空，若为空则使用条件变量阻塞等待，并释放锁
        while (_msg_que.empty() && !_b_stop) {
            _consume.wait(lock);
        }
        // 判断是否为关闭状态，将所有逻辑执行完成之后退出循环
        if (_b_stop) {
            while (!_msg_que.empty()) {
                auto msg_node = _msg_que.front();
                LOG(INFO) << "Recv_Msg id is: " << msg_node->_recv_node->_msg_id;
                auto call_back_iter = _func_callbacks.find(msg_node->_recv_node->_msg_id);
                if (_func_callbacks.end() == call_back_iter) {
                    _msg_que.pop();
                    continue;
                }
                call_back_iter->second(msg_node->_session, msg_node->_recv_node->_msg_id,
                                       std::string(msg_node->_recv_node->_data, msg_node->_recv_node->_cur_len));
                _msg_que.pop();
            }
            break;
        }
        // 若没有停服，则说明队列有数据
        auto msg_node = _msg_que.front();
        LOG(INFO) << "Recv_Msg id is: " << msg_node->_recv_node->_msg_id;
        auto call_back_iter = _func_callbacks.find(msg_node->_recv_node->_msg_id);
        if (_func_callbacks.end() == call_back_iter) {
            _msg_que.pop();
            LOG(INFO) << "msg_id [" << msg_node->_recv_node->_msg_id << "] handler not found.";
            continue;
        }
        call_back_iter->second(msg_node->_session, msg_node->_recv_node->_msg_id,
                               std::string(msg_node->_recv_node->_data, msg_node->_recv_node->_cur_len));
        _msg_que.pop();
    }
}

void LogicSystem::RegisterCallBacks() {
    _func_callbacks[MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LoginHandler, this,
                                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    _func_callbacks[ID_SEARCH_USER_REQ] = std::bind(&LogicSystem::SearchInfo, this, std::placeholders::_1,
                                                    std::placeholders::_2, std::placeholders::_3);
    _func_callbacks[ID_ADD_FRIEND_REQ] = std::bind(&LogicSystem::AddFriendApply, this, std::placeholders::_1,
                                                   std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::LoginHandler(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data) {
    Json::Value root;
    Json::Reader reader;
    reader.parse(msg_data, root);
    auto uid = root["uid"].asInt();
    auto token = root["token"].asString();
    LOG(INFO) << "user login uid is: " << uid;
    LOG(INFO) << "user token is: " << root["token"].asString();

    Json::Value rtvalue;
    Defer defer([this, &rtvalue, session]() {
        std::string return_str = rtvalue.toStyledString();
        session->Send(return_str, MSG_CHAT_LOGIN_RSP);
    });

    // 从redis获取用户token是否正确
    std::string uid_str = std::to_string(uid);
    std::string token_key = USERTOKENPREFIX + uid_str;
    std::string token_value = "";
    bool success = RedisManager::GetInstance()->Get(token_key, token_value);
    if (!success) {
        rtvalue["error"] = ErrorCodes::UidInvalid;
        return;
    }
    if (token_value != token) {
        rtvalue["error"] = ErrorCodes::TokenInvalid;
        return;
    }
    rtvalue["error"] = ErrorCodes::Success;

    std::string base_key = USER_BASE_INFO + uid_str;
    auto user_info = std::make_shared<UserInfo>();
    bool b_base = GetBaseInfo(base_key, uid, user_info);
    if (!b_base) {
        rtvalue["error"] = ErrorCodes::UidInvalid;
        return;
    }
    rtvalue["uid"] = uid;
    rtvalue["pwd"] = user_info->pwd;
    rtvalue["name"] = user_info->name;
    rtvalue["email"] = user_info->email;
    rtvalue["nick"] = user_info->nick;
    rtvalue["desc"] = user_info->desc;
    rtvalue["sex"] = user_info->sex;
    rtvalue["icon"] = user_info->icon;

    // 从数据库获取好友申请列表
    std::vector<std::shared_ptr<ApplyInfo>> apply_list;
    auto b_apply = GetFriendApplyInfo(uid, apply_list);
    if (b_apply) {
        for (auto &apply: apply_list) {
            Json::Value obj;
            obj["name"] = apply->_name;
            obj["uid"] = apply->_uid;
            obj["icon"] = apply->_icon;
            obj["nick"] = apply->_nick;
            obj["sex"] = apply->_sex;
            obj["desc"] = apply->_desc;
            obj["status"] = apply->_status;
            rtvalue["apply_list"].append(obj);
        }
    }

    // 从数据库获取好友列表
    auto server_name = ConfigManager::Inst().GetValue("SelfServer", "Name");

    // 将登录数量增加
    auto rd_res = RedisManager::GetInstance()->HGet(LOGIN_COUNT, server_name);
    int count = 0;
    if (!rd_res.empty()) {
        count = std::stoi(rd_res);
    }
    count++;

    // 将登录数量写入 redis
    auto count_str = std::to_string(count);
    RedisManager::GetInstance()->HSet(LOGIN_COUNT, server_name, count_str);

    // session 绑定用户
    session->SetUserId(uid);
    // 为用户设置登录 ip Server 的名称
    std::string ipKey = USERIPPREFIX + uid_str;
    RedisManager::GetInstance()->Set(ipKey, server_name);
    // uid 和 session 绑定管理，方便后续踢人操作
    UserManager::GetInstance()->SetUserSession(uid, session);

    return;
}

bool LogicSystem::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo> &userinfo) {
    // 优先通过 redis 查询用户信息
    std::string info_str = "";
    bool b_base = RedisManager::GetInstance()->Get(base_key, info_str);
    if (b_base) {
        Json::Reader reader;
        Json::Value root;
        reader.parse(info_str, root);
        userinfo->uid = root["uid"].asInt();
        userinfo->name = root["name"].asString();
        userinfo->pwd = root["pwd"].asString();
        userinfo->email = root["email"].asString();
        userinfo->nick = root["nick"].asString();
        userinfo->desc = root["desc"].asString();
        userinfo->sex = root["sex"].asInt();
        userinfo->icon = root["icon"].asString();
        LOG(INFO) << "user login uid is  " << userinfo->uid << " name  is "
                  << userinfo->name << " pwd is " << userinfo->pwd << " email is " << userinfo->email;
    } else {
        //　redis 之中没有，则查询 mysql
        std::shared_ptr<UserInfo> user_info = nullptr;
        user_info = MysqlManager::GetInstance()->GetUserByUid(uid);
        if (user_info == nullptr) {
            return false;
        }
        userinfo = user_info;

        Json::Value redis_root;
        redis_root["uid"] = uid;
        redis_root["pwd"] = userinfo->pwd;
        redis_root["name"] = userinfo->name;
        redis_root["email"] = userinfo->email;
        redis_root["nick"] = userinfo->nick;
        redis_root["desc"] = userinfo->desc;
        redis_root["sex"] = userinfo->sex;
        redis_root["icon"] = userinfo->icon;
        RedisManager::GetInstance()->Set(base_key, redis_root.toStyledString());
    }
    return true;
}

void LogicSystem::GetUserByName(std::string name, Json::Value &rtvalue) {
    rtvalue["error"] = ErrorCodes::Success;

    std::string base_key = NAME_INFO + name;

    // 优先通过 redis 查询用户信息
    std::string info_str = "";
    bool b_base = RedisManager::GetInstance()->Get(base_key, info_str);
    if (b_base) {
        Json::Reader reader;
        Json::Value root;
        reader.parse(info_str, root);
        auto uid = root["uid"].asInt();
        auto name = root["name"].asString();
        auto pwd = root["pwd"].asString();
        auto email = root["email"].asString();
//        auto nick = root["nick"].asString();
//        auto desc = root["desc"].asString();
//        auto sex = root["sex"].asInt();
        LOG(INFO) << "user  uid is  " << uid << " name  is "
                  << name << " pwd is " << pwd << " email is " << email;;

        rtvalue["uid"] = uid;
        rtvalue["pwd"] = pwd;
        rtvalue["name"] = name;
        rtvalue["email"] = email;
//        rtvalue["nick"] = nick;
//        rtvalue["desc"] = desc;
//        rtvalue["sex"] = sex;
        return;
    }

    // redis 之中没有，则查询 mysql 数据库
    std::shared_ptr<UserInfo> user_info = nullptr;
    user_info = MysqlManager::GetInstance()->GetUser(name);
    if (user_info == nullptr) {
        rtvalue["error"] = ErrorCodes::UidInvalid;
        return;
    }

    // 将数据写入 redis
    Json::Value redis_root;
    redis_root["uid"] = user_info->uid;
    redis_root["pwd"] = user_info->pwd;
    redis_root["name"] = user_info->name;
    redis_root["email"] = user_info->email;
//    redis_root["nick"] = user_info->nick;
//    redis_root["desc"] = user_info->desc;
//    redis_root["sex"] = user_info->sex;

    RedisManager::GetInstance()->Set(base_key, redis_root.toStyledString());

    // 返回数据
    rtvalue["uid"] = user_info->uid;
    rtvalue["pwd"] = user_info->pwd;
    rtvalue["name"] = user_info->name;
    rtvalue["email"] = user_info->email;
//    rtvalue["nick"] = user_info->nick;
//    rtvalue["desc"] = user_info->desc;
//    rtvalue["sex"] = user_info->sex;
}


void LogicSystem::SearchInfo(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data) {
    Json::Reader reader;
    Json::Value root;
    reader.parse(msg_data, root);
    auto uid_str = root["uid"].asString();
    LOG(INFO) << "user SearchInfo uid is  " << uid_str;;

    Json::Value rtvalue;

    Defer defer([this, &rtvalue, session]() {
        std::string return_str = rtvalue.toStyledString();
        session->Send(return_str, ID_SEARCH_USER_RSP);
    });

    bool b_digit = isPureDigit(uid_str);
    if (b_digit) {
        GetUserByUid(uid_str, rtvalue);
    } else {
        GetUserByName(uid_str, rtvalue);
    }
    return;
}

bool LogicSystem::isPureDigit(const std::string &str) {
    for (char c: str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

void LogicSystem::GetUserByUid(std::string uid_str, Json::Value &rtvalue) {
    rtvalue["error"] = ErrorCodes::Success;

    std::string base_key = USER_BASE_INFO + uid_str;

    // 优先通过 redis 查询用户信息
    std::string info_str = "";
    bool b_base = RedisManager::GetInstance()->Get(base_key, info_str);
    if (b_base) {
        Json::Reader reader;
        Json::Value root;
        reader.parse(info_str, root);
        auto uid = root["uid"].asInt();
        auto name = root["name"].asString();
        auto pwd = root["pwd"].asString();
        auto email = root["email"].asString();
//        auto nick = root["nick"].asString();
//        auto desc = root["desc"].asString();
//        auto sex = root["sex"].asInt();
//        auto icon = root["icon"].asString();
        //LOG(INFO) << "user  uid is  " << uid << " name  is "<< name << " pwd is " << pwd << " email is " << email << " icon is " << icon;;

        rtvalue["uid"] = uid;
        rtvalue["pwd"] = pwd;
        rtvalue["name"] = name;
        rtvalue["email"] = email;
//        rtvalue["nick"] = nick;
//        rtvalue["desc"] = desc;
//        rtvalue["sex"] = sex;
//        rtvalue["icon"] = icon;
        return;
    }

    auto uid = std::stoi(uid_str);
    // redis 之中不存在，则查询 mysql 数据库
    std::shared_ptr<UserInfo> user_info = nullptr;
    user_info = MysqlManager::GetInstance()->GetUser(uid);
    if (user_info == nullptr) {
        rtvalue["error"] = ErrorCodes::UidInvalid;
        return;
    }

    // 将数据库内容写入 redis 缓存
    Json::Value redis_root;
    redis_root["uid"] = user_info->uid;
    redis_root["pwd"] = user_info->pwd;
    redis_root["name"] = user_info->name;
    redis_root["email"] = user_info->email;
//    redis_root["nick"] = user_info->nick;
//    redis_root["desc"] = user_info->desc;
//    redis_root["sex"] = user_info->sex;
//    redis_root["icon"] = user_info->icon;

    RedisManager::GetInstance()->Set(base_key, redis_root.toStyledString());

    // 返回数据
    rtvalue["uid"] = user_info->uid;
    rtvalue["pwd"] = user_info->pwd;
    rtvalue["name"] = user_info->name;
    rtvalue["email"] = user_info->email;
//    rtvalue["nick"] = user_info->nick;
//    rtvalue["desc"] = user_info->desc;
//    rtvalue["sex"] = user_info->sex;
//    rtvalue["icon"] = user_info->icon;
}

void LogicSystem::AddFriendApply(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data) {
    Json::Reader reader;
    Json::Value root;
    reader.parse(msg_data, root);
    auto uid = root["uid"].asInt();
    auto applyname = root["applyname"].asString();
    auto bakname = root["bakname"].asString();
    auto touid = root["touid"].asInt();
    LOG(INFO) << "user login uid is  " << uid << " applyname  is "
              << applyname << " bakname is " << bakname << " touid is " << touid;

    Json::Value rtvalue;
    rtvalue["error"] = ErrorCodes::Success;
    Defer defer([this, &rtvalue, session]() {
        std::string return_str = rtvalue.toStyledString();
        session->Send(return_str, ID_ADD_FRIEND_RSP);
    });
    // 先更新数据库
    MysqlManager::GetInstance()->AddFriendApply(uid, touid);
    // 查询 redis 查找 touid 对应的 server ip
    auto to_str = std::to_string(touid);
    auto to_ip_key = USERIPPREFIX + to_str;
    std::string to_ip_value = "";
    bool b_ip = RedisManager::GetInstance()->Get(to_ip_key, to_ip_value);
    if (!b_ip) {
        return;
    }

    auto &cfg = ConfigManager::Inst();
    auto self_name = cfg["SelfServer"]["Name"];
    // 出现申请消息，直接通知对方
    if (to_ip_value == self_name) {
        auto session = UserManager::GetInstance()->GetSession(touid);
        if (session) {
            // 在内存之中直接发送通知对方
            Json::Value notify;
            notify["error"] = ErrorCodes::Success;
            notify["applyuid"] = uid;
            notify["name"] = applyname;
            notify["desc"] = "";
            std::string return_str = notify.toStyledString();
            session->Send(return_str, ID_NOTIFY_ADD_FRIEND_REQ);
        }

        return;
    }
    std::string base_key = USER_BASE_INFO + std::to_string(uid);
    auto apply_info = std::make_shared<UserInfo>();
    bool b_info = GetBaseInfo(base_key, uid, apply_info);

    AddFriendReq add_req;
    add_req.set_applyuid(uid);
    add_req.set_touid(touid);
    add_req.set_name(applyname);
    add_req.set_desc("");
    if (b_info) {
        add_req.set_icon(apply_info->icon);
        add_req.set_sex(apply_info->sex);
        add_req.set_nick(apply_info->nick);
    }
    // 发送通知
    ChatGrpcClient::GetInstance()->NotifyAddFriend(to_ip_value, add_req);
}

bool LogicSystem::GetFriendApplyInfo(int to_uid, std::vector<std::shared_ptr<ApplyInfo>> &list) {
    return MysqlManager::GetInstance()->GetApplyList(to_uid, list, 0, 10);
}
