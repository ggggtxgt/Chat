#include <QJsonArray>

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

std::vector<std::shared_ptr<ApplyInfo>> UserManager::GetApplyList() {
    return _apply_list;
}

void UserManager::AddApplyList(std::shared_ptr<ApplyInfo> app) {
    _apply_list.push_back(app);
}

bool UserManager::AlreadyApply(int uid) {
    for (auto &apply: _apply_list) {
        if (apply->_uid == uid) {
            return true;
        }
    }
    return false;
}

void UserManager::SetUserInfo(std::shared_ptr<UserInfo> user_info) {
    _user_info = user_info;
}

void UserManager::AppendApplyList(QJsonArray array) {
    // 遍历 QJsonArray 并输出每个元素
    for (const QJsonValue &value: array) {
        auto name = value["name"].toString();
        auto desc = value["desc"].toString();
        auto icon = value["icon"].toString();
        auto nick = value["nick"].toString();
        auto sex = value["sex"].toInt();
        auto uid = value["uid"].toInt();
        auto status = value["status"].toInt();
        auto info = std::make_shared<ApplyInfo>(uid, name, desc, icon, nick, sex, status);
        _apply_list.push_back(info);
    }
}
