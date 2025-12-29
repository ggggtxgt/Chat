#ifndef GATESERVER_REDISMANAGER_H
#define GATESERVER_REDISMANAGER_H

#include <queue>
#include <assert.h>
#include <hiredis.h>

#include "Const.h"
#include "RedisConnPool.h"

class RedisManager : public Singleton<RedisManager> {
    friend class Singleton<RedisManager>;

public:
    ~RedisManager();                                                            // 析构函数
    bool Get(const std::string &key, std::string &value);                       // 获取指定 key 的字符串值
    bool Set(const std::string &key, const std::string &value);                 // 设置指定 key 的字符串值
    bool Auth(const std::string &password);                                     // 输入 Redis 密码
    bool LPush(const std::string &key, const std::string &value);               // 从列表左侧插入元素
    bool LPop(const std::string &key, std::string &value);                      // 从列表左侧弹出元素
    bool RPush(const std::string &key, const std::string &value);               // 从列表右侧插入元素
    bool RPop(const std::string &key, std::string &value);                      // 从列表右侧弹出元素
    bool HSet(const std::string &, const std::string &, const std::string &);   // 设置哈希表中指定字段的值（字符串）
    bool HSet(const char *, const char *, const char *, std::size_t);           // 设置哈希表中指定字段的值（二进制）
    std::string HGet(const std::string &, const std::string &);                 // 获取哈希表中指定字段的值
    bool Del(const std::string &key);                                           // 删除指定的 key
    bool ExistsKey(const std::string &key);                                     // 检查指定的 key 是否存在
    void Close();                                                               // 关闭 Redis 连接，释放资源

private:
    RedisManager();

private:
    std::unique_ptr<RedisConnPool> _conn_pool;
};


#endif //GATESERVER_REDISMANAGER_H
