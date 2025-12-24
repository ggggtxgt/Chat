#include "Glog.h"
#include "RedisConnPool.h"

RedisConnPool::RedisConnPool(std::size_t size, const char *host, int port, const char *pwd) :
        size_(size), host_(host), port_(port), b_stop_(false) {
    // 创建连接
    for (std::size_t i = 0; i < size_; i++) {
        auto *context = redisConnect(host, port);
        if (nullptr == context || context->err != 0) {
            LOG(ERROR) << "Redis connection failed: "
                       << (context ? context->errstr : "unknown error");
            if (nullptr != context) {
                redisFree(context);
            }
            continue;
        }
        // 如果设置了密码，进行认证
        if (pwd && strlen(pwd) > 0) {
            auto reply = (redisReply *) redisCommand(context, "AUTH %s", pwd);
            if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
                LOG(ERROR) << "Redis authentication failed: "
                           << (reply ? reply->str : "unknown error");
                if (reply) freeReplyObject(reply);
                redisFree(context);
                continue;
            }
            freeReplyObject(reply);
            LOG(INFO) << "Redis authentication successful";
        }
        connections_.push(context);
        LOG(INFO) << "Redis connection " << i + 1 << "/" << size_ << " created";
    }

    if (connections_.empty()) {
        LOG(ERROR) << "Failed to create any Redis connection";
    } else {
        LOG(INFO) << "Redis connection pool initialized with "
                  << connections_.size() << " connections";
    }
}

RedisConnPool::~RedisConnPool() {
    Close();

    std::lock_guard<std::mutex> lock(mutex_);
    while (!connections_.empty()) {
        auto context = connections_.front();
        connections_.pop();
        if (context) {
            redisFree(context);
        }
    }
}

redisContext *RedisConnPool::GetConnection() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this]() {
        return b_stop_ || !connections_.empty();
    });
    if (b_stop_) {
        return nullptr;
    }
    auto *context = connections_.front();
    connections_.pop();
    return context;
}

void RedisConnPool::ReturnConnection(redisContext *context) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (b_stop_ || context == nullptr) return;
    connections_.push(context);
    cond_.notify_one();
}

void RedisConnPool::Close() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        b_stop_ = true;
    }
    cond_.notify_all();
}