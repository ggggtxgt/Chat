#include "Glog.h"
#include "RedisManager.h"

RedisManager::RedisManager() {}

RedisManager::~RedisManager() {}

bool RedisManager::Connect(const std::string &host, int port) {
    this->_connect = redisConnect(host.c_str(), port);

    // 处理连接失败的情况
    if (this->_connect == nullptr) return false;
    if (this->_connect != nullptr && this->_connect->err) {
        LOG(ERROR) << "connect error" << this->_connect->errstr;
        return false;
    }
    return true;
}

bool RedisManager::Get(const std::string &key, std::string &value) {
    this->_reply = (redisReply *) redisCommand(this->_connect, "GET %s", key.c_str());
    if (this->_reply == nullptr) {
        LOG(ERROR) << "[Get " << key << "] failed.";
        freeReplyObject(this->_reply);
        return false;
    }
    if (this->_reply->type != REDIS_REPLY_STRING) {
        LOG(ERROR) << " [GET " << key << "] failed !";
        freeReplyObject(this->_reply);
        return false;
    }

    value = this->_reply->str;
    freeReplyObject(this->_reply);
   LOG(INFO) << "Succeed to execute command [Get " << key << "] ";
    return true;
}

bool RedisManager::Set(const std::string &key, const std::string &value) {
    // 执行 redis 命令行
    this->_reply = (redisReply *) redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
    // 如果返回 NULL，则说明执行失败
    if (nullptr == this->_reply) {
        LOG(ERROR) << "Execute command [SET " << key << " " << value << "] failed!";
        freeReplyObject(this->_reply);
        return false;
    }
    // 如果执行失败，则释放连接
    if (!(this->_reply->type == REDIS_REPLY_STATUS &&
          (strcmp(this->_reply->str, "OK") == 0 || strcmp(this->_reply->str, "ok") == 0))) {
        LOG(ERROR) << "Execute command [SET " << key << " " << value << "] failed!";
        freeReplyObject(this->_reply);
        return false;
    }

    // 执行成功，释放 redisCommand 执行后返回的 redisReply 所占用的内存
    freeReplyObject(this->_reply);
    LOG(INFO) << "Execute command [SET " << key << " " << value << "] success!";
    return true;
}

bool RedisManager::Auth(const std::string &password) {
    this->_reply = (redisReply *) redisCommand(this->_connect, "AUTH %s", password.c_str());
    if (this->_reply->type == REDIS_REPLY_ERROR) {
        LOG(ERROR) << "认证失败";
        // 执行成功，释放 redisCommand 执行后返回的 redisReply 所占用的内存
        freeReplyObject(this->_reply);
        return false;
    } else {
        // 执行成功，释放 redisCommand 执行之后返回的 redisReply 所占用的内存
        freeReplyObject(this->_reply);
        LOG(INFO) << "认证成功";
        return true;
    }
}

bool RedisManager::LPush(const std::string &key, const std::string &value) {
    this->_reply = (redisReply *) redisCommand(this->_connect, "LPUSH %s %s", key.c_str(), value.c_str());
    if (nullptr == this->_reply) {
        LOG(ERROR) << "Execute command [LPUSH " << key << " " << value << "] failure !";
        freeReplyObject(this->_reply);
        return false;
    }
    if (this->_reply->type != REDIS_REPLY_INTEGER || this->_reply->integer <= 0) {
        LOG(ERROR) << "Execute command [LPUSH " << key << " " << value << "] failure !";
        freeReplyObject(this->_reply);
        return false;
    }
    LOG(INFO) << "Execute command [LPUSH " << key << " " << value << "] success !";
    freeReplyObject(this->_reply);
    return true;
}

bool RedisManager::LPop(const std::string &key, std::string &value) {
    this->_reply = (redisReply *) redisCommand(this->_connect, "LPOP %s ", key.c_str());
    if (_reply == nullptr || _reply->type == REDIS_REPLY_NIL) {
        LOG(ERROR) << "Execute command [LPOP " << key << "] failure !";
        freeReplyObject(this->_reply);
        return false;
    }
    value = _reply->str;
    LOG(INFO) << "Execute command [LPOP " << key << "] success !";
    freeReplyObject(this->_reply);
    return true;
}

bool RedisManager::RPush(const std::string &key, const std::string &value) {
    this->_reply = (redisReply *) redisCommand(this->_connect, "RPUSH %s %s", key.c_str(), value.c_str());
    if (NULL == this->_reply) {
        LOG(ERROR) << "Execute command [RPUSH " << key << "  " << value << "] failure !";
        freeReplyObject(this->_reply);
        return false;
    }
    if (this->_reply->type != REDIS_REPLY_INTEGER || this->_reply->integer <= 0) {
        LOG(ERROR) << "Execute command [RPUSH " << key << "  " << value << "] failure !";
        freeReplyObject(this->_reply);
        return false;
    }
    LOG(INFO) << "Execute command [RPUSH " << key << "  " << value << "] success !";
    freeReplyObject(this->_reply);
    return true;
}

bool RedisManager::RPop(const std::string &key, std::string &value) {
    this->_reply = (redisReply *) redisCommand(this->_connect, "RPOP %s ", key.c_str());
    if (_reply == nullptr || _reply->type == REDIS_REPLY_NIL) {
        LOG(ERROR) << "Execute command [RPOP " << key << "] failure !";
        freeReplyObject(this->_reply);
        return false;
    }
    value = _reply->str;
    LOG(INFO) << "Execute command [RPOP " << key << "] success !";
    freeReplyObject(this->_reply);
    return true;
}

// 便于处理字符串
bool RedisManager::HSet(const std::string &key, const std::string &hkey, const std::string &value) {
    this->_reply = (redisReply *) redisCommand(this->_connect, "HSET %s %s %s", key.c_str(), hkey.c_str(),
                                               value.c_str());
    if (_reply == nullptr || _reply->type != REDIS_REPLY_INTEGER) {
        LOG(ERROR) << "Execute command [HSet " << key << "  " << hkey << "  " << value << "] failure !";
        freeReplyObject(this->_reply);
        return false;
    }
    LOG(INFO) << "Execute command [HSet " << key << "  " << hkey << "  " << value << "] success !";
    freeReplyObject(this->_reply);
    return true;
}

// 便于处理二进制文件
bool RedisManager::HSet(const char *key, const char *hkey, const char *hvalue, size_t hvaluelen) {
    const char *argv[4];
    size_t argvlen[4];
    argv[0] = "HSET";
    argvlen[0] = 4;
    argv[1] = key;
    argvlen[1] = strlen(key);
    argv[2] = hkey;
    argvlen[2] = strlen(hkey);
    argv[3] = hvalue;
    argvlen[3] = hvaluelen;
    this->_reply = (redisReply *) redisCommandArgv(this->_connect, 4, argv, argvlen);
    if (_reply == nullptr || _reply->type != REDIS_REPLY_INTEGER) {
        LOG(ERROR) << "Execute command [HSet " << key << "  " << hkey << "  " << hvalue << "] failure !";
        freeReplyObject(this->_reply);
        return false;
    }
    LOG(INFO) << "Execute command [HSet " << key << "  " << hkey << "  " << hvalue << "] success !";
    freeReplyObject(this->_reply);
    return true;
}

std::string RedisManager::HGet(const std::string &key, const std::string &hkey) {
    const char *argv[3];
    size_t argvlen[3];
    argv[0] = "HGET";
    argvlen[0] = 4;
    argv[1] = key.c_str();
    argvlen[1] = key.length();
    argv[2] = hkey.c_str();
    argvlen[2] = hkey.length();
    this->_reply = (redisReply *) redisCommandArgv(this->_connect, 3, argv, argvlen);
    if (this->_reply == nullptr || this->_reply->type == REDIS_REPLY_NIL) {
        freeReplyObject(this->_reply);
        LOG(ERROR) << "Execute command [HGet " << key << " " << hkey << "  ] failure !";
        return "";
    }

    std::string value = this->_reply->str;
    freeReplyObject(this->_reply);
    LOG(INFO) << "Execute command [HGet " << key << " " << hkey << "] success !";
    return value;
}

bool RedisManager::Del(const std::string &key) {
    this->_reply = (redisReply *) redisCommand(this->_connect, "DEL %s", key.c_str());
    if (this->_reply == nullptr || this->_reply->type != REDIS_REPLY_INTEGER) {
        LOG(ERROR) << "Execute command [Del " << key << "] failure !";
        freeReplyObject(this->_reply);
        return false;
    }
    LOG(INFO) << "Execute command [Del " << key << "] success !";
    freeReplyObject(this->_reply);
    return true;
}

bool RedisManager::ExistsKey(const std::string &key) {
    // 通过 redis 中的 exists 进行判断
    this->_reply = (redisReply *) redisCommand(this->_connect, "exists %s", key.c_str());
    if (this->_reply == nullptr || this->_reply->type != REDIS_REPLY_INTEGER || this->_reply->integer == 0) {
        LOG(INFO) << "Not Found [Key " << key << "] !";
        freeReplyObject(this->_reply);
        return false;
    }
    LOG(INFO) << " Found [Key " << key << "] exists !";
    freeReplyObject(this->_reply);
    return true;
}

void RedisManager::Close() {
    redisFree(_connect);
}