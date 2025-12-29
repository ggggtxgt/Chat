#include "usermanager.h"

UserManager::UserManager() {}

UserManager::~UserManager() noexcept {}

void UserManager::SetUid(int uid) { _uid = uid; }

void UserManager::SetName(QString name) { _name = name; }

void UserManager::SetToken(QString token) { _token = token; }