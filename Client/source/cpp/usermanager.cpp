#include "usermanager.h"

UserManager::UserManager() {}

UserManager::~UserManager() noexcept {}

void UserManager::SetUid(int uid) { _uid = uid; }

void UserManager::SetName(QString name) { _name = name; }

void UserManager::SetToken(QString token) { _token = token; }

QString UserManager::GetName() {
    return _name;
}

int UserManager::GetUid() {
    return _uid;
}

void UserManager::UpdateContactLoadedCount() {
    int begin = _contact_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;

    if (begin >= _friend_list.size()) {
        return;
    }
    if (end > _friend_list.size()) {
        _contact_loaded = _friend_list.size();
        return;
    }
    _contact_loaded = end;
}

std::vector<std::shared_ptr<FriendInfo>> UserManager::GetConListPerPage() {
    std::vector<std::shared_ptr<FriendInfo>> friend_list;
    int begin = _contact_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;

    if (begin >= _friend_list.size()) {
        return friend_list;
    }
    if (end > _friend_list.size()) {
        friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin, _friend_list.end());
        return friend_list;
    }
    friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin, _friend_list.begin() + end);
    return friend_list;
}

bool UserManager::IsLoadChatFin() {
    if (_chat_loaded >= _friend_list.size()) {
        return true;
    }
    return false;
}