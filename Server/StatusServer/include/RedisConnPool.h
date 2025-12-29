#ifndef GATESERVER_REDISCONNPOOL_H
#define GATESERVER_REDISCONNPOOL_H

#include <queue>

#include "Const.h"

#include "hiredis.h"

class RedisConnPool {
public:
    RedisConnPool(std::size_t, const char *, int port, const char *);   // 构造函数
    ~RedisConnPool();                                                   // 析构函数
    redisContext *GetConnection();                                      // 获取 redis 连接
    void ReturnConnection(redisContext *context);                       // 归还 redis 连接
    void Close();                                                       // 关闭 redisConnPool

private:
    int port_;
    const char *host_;
    std::size_t size_;
    std::mutex mutex_;
    std::atomic<bool> b_stop_;
    std::condition_variable cond_;
    std::queue<redisContext *> connections_;
};


#endif //GATESERVER_REDISCONNPOOL_H
