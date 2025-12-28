#include "MysqlManager.h"

MysqlManager::~MysqlManager() {}

MysqlManager::MysqlManager() {}

int MysqlManager::RegisterUser(const std::string &name, const std::string &email, const std::string &pwd) {
    return _dao.RegisterUser(name, email, pwd);
}

bool MysqlManager::CheckEmail(const std::string &name, const std::string &email) {
    return _dao.CheckEmail(name, email);
}

bool MysqlManager::UpdatePwd(const std::string &name, const std::string &pwd) {
    return _dao.UpdatePwd(name, pwd);
}

int MysqlManager::GetUserIdByName(const std::string &name) {
    return _dao.GetUserIdByName(name);
}
