#ifndef GATESERVER_MYSQLDAO_H
#define GATESERVER_MYSQLDAO_H

#include "Data.h"
#include "Const.h"

#include <mutex>
#include <queue>
#include <thread>

#include "jdbc/mysql_driver.h"
#include "jdbc/mysql_connection.h"
#include "jdbc/cppconn/resultset.h"
#include "jdbc/cppconn/statement.h"
#include "jdbc/cppconn/exception.h"
#include "jdbc/cppconn/prepared_statement.h"

class SqlConnection {
public:
    SqlConnection(sql::Connection *con, int64_t lasttime) : _con(con), _last_time(lasttime) {}

    int64_t _last_time;
    std::unique_ptr<sql::Connection> _con;
};

class MysqlConnPool {
public:
    MysqlConnPool(const std::string &, const std::string &, const std::string &,
                  const std::string &, int);                // 构造函数
    ~MysqlConnPool();                                       // 析构函数
    void CheckConnection();                                 // 检测连接是否存活
    std::unique_ptr<SqlConnection> GetConnection();         // 获取 Mysql 连接
    void ReturnConnection(std::unique_ptr<SqlConnection>);  // 归还 Mysql 连接
    void Close();                                           // 关闭 MysqlConnPool

private:
    int size_;
    std::string url_;
    std::string user_;
    std::string passwd_;
    std::string schema_;

    std::mutex mutex_;
    std::thread _check_thread;
    std::atomic<bool> b_stop_;
    std::condition_variable cond_;
    std::queue<std::unique_ptr<SqlConnection>> sqlpool_;
};

class MysqlDao {
public:
    MysqlDao();                                                                     // 构造函数
    ~MysqlDao();                                                                    // 析构函数
    bool CheckEmail(const std::string &, const std::string &);                      // 检查用户邮箱
    int RegisterUser(const std::string &, const std::string &, const std::string &);// 录入用户信息
    bool UpdatePwd(const std::string &, const std::string &);                       // 更新用户密码
    int GetUserIdByName(const std::string &name);                                   // 根据姓名查询 uid
    bool CheckPwd(const std::string &, const std::string &, UserInfo &);            // 判断密码是否正确
    std::shared_ptr<UserInfo> GetUserByUid(int uid);                                // 根据 uid 获取用户信息

private:
    std::unique_ptr<MysqlConnPool> conpool_;
};


#endif //GATESERVER_MYSQLDAO_H
