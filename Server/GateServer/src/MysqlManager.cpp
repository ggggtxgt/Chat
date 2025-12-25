#include "MysqlManager.h"

MysqlManager::~MysqlManager() {}

MysqlManager::MysqlManager() {}

int MysqlManager::RegisterUser(const std::string &name, const std::string &email, const std::string &pwd) {
    return _dao.RegisterUser(name, email, pwd);
}