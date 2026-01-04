#ifndef GATESERVER_MYSQLMANAGER_H
#define GATESERVER_MYSQLMANAGER_H

#include "Const.h"
#include "MysqlDao.h"
#include "Singleton.h"

class MysqlManager : public Singleton<MysqlManager> {
    friend class Singleton<MysqlManager>;

public:
    ~MysqlManager();                                                                     // 析构函数
    bool CheckEmail(const std::string &, const std::string &);                           // 调用 MySQLDao 检查用户邮箱
    int RegisterUser(const std::string &, const std::string &, const std::string &);     // 调用 MySQLDao 录入用户信息
    bool UpdatePwd(const std::string &, const std::string &);                            // 调用 MySQLDao 更新用户密码
    bool CheckPwd(const std::string &email, const std::string &pwd, UserInfo &userInfo); // 调用 MySQLDao 判断密码是否正确
    int GetUserIdByName(const std::string &);                                            // 调用 MySQLDao 根据姓名查询 uid
    std::shared_ptr<UserInfo> GetUserByUid(int uid);                                     // 调用 MySQLDao 根据 uid 查询用户信息

private:
    MysqlManager();

    MysqlDao _dao;
};


#endif //GATESERVER_MYSQLMANAGER_H
