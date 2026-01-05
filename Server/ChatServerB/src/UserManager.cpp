#include "CSession.h"
#include "UserManager.h"

UserManager::UserManager() {}

UserManager::~UserManager() {
    _uid_to_session.clear();
}

std::shared_ptr<CSession> UserManager::GetSession(int uid) {
    std::lock_guard<std::mutex> lock(_session_mtx);
    auto iter = _uid_to_session.find(uid);
    if (iter == _uid_to_session.end()) {
        return nullptr;
    }
    return iter->second;
}

void UserManager::SetUserSession(int uid, std::shared_ptr<CSession> session) {
    std::lock_guard<std::mutex> lock(_session_mtx);
    _uid_to_session[uid] = session;
}

void UserManager::RmvUserSession(int uid, std::string session_id) {
    auto uid_str = std::to_string(uid);
    // 再次登录可能是其他服务器，可能造成栈服务器删除 key，其他服务器注册 key的情况
    // 可能出现其他情况登录，栈服务删除key造成找不到key的情况
    {
        std::lock_guard<std::mutex> lock(_session_mtx);
        _uid_to_session.erase(uid);
    }
}
