#ifndef GATESERVER_MYSQLMANAGER_H
#define GATESERVER_MYSQLMANAGER_H

#include "Const.h"
#include "MysqlDao.h"

class MysqlManager : public Singleton<MysqlManager> {
    friend class Singleton<MysqlManager>;

public:
    ~MysqlManager();                            // 析构函数
    int RegisterUser(const std::string &, const std::string &,
                     const std::string &);      // 调用 MySQLDao 录入用户信息

private:
    MysqlManager();

    MysqlDao _dao;
};


#endif //GATESERVER_MYSQLMANAGER_H
