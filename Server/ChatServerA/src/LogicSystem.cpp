#include "Glog.h"
#include "Const.h"
#include "LogicSystem.h"
#include "MysqlManager.h"
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
    LOG(INFO) << "user login uid is: " << uid << " user token  is: " << root["token"].asString();
    // 从状态服务器获取token匹配是否正确
    auto rsp = StatusGrpcClient::GetInstance()->Login(uid, root["token"].asString());
    Json::Value rtvalue;
    Defer defer([this, &rtvalue, session]() {
        std::string return_str = rtvalue.toStyledString();
        session->Send(return_str, MSG_CHAT_LOGIN_RSP);
    });

    rtvalue["error"] = rsp.error();
    if (rsp.error() != ErrorCodes::Success) {return;}

    // 内存中查询用户信息
    auto find_iter = _users.find(uid);
    std::shared_ptr<UserInfo> user_info = nullptr;
    if (find_iter == _users.end()) {
        // 查询数据库
        user_info = MysqlManager::GetInstance()->GetUserByUid(uid);
        if (user_info == nullptr) {
            rtvalue["error"] = ErrorCodes::UidInvalid;
            return;
        }
        _users[uid] = user_info;
    } else {
        user_info = find_iter->second;
    }
    rtvalue["uid"] = uid;
    rtvalue["token"] = rsp.token();
    rtvalue["name"] = user_info->name;
}