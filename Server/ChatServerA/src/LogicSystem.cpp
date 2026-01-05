#include "Glog.h"
#include "Const.h"
#include "UserManager.h"
#include "LogicSystem.h"
#include "MysqlManager.h"
#include "RedisManager.h"
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