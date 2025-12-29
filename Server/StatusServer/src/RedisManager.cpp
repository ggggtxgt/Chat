#include "Glog.h"
#include "RedisManager.h"
#include "ConfigManager.h"
#include <cstring>

// Windows 和 Linux 兼容性处理
#ifdef _WIN32
#define STR_CMP _stricmp
#else
#define STR_CMP strcasecmp
#endif

RedisManager::RedisManager() {
    auto &gCfgMgr = ConfigManager::Inst();
    auto host = gCfgMgr["Redis"]["Host"];
    auto port = gCfgMgr["Redis"]["Port"];
    auto pwd = gCfgMgr["Redis"]["Passwd"];
    _conn_pool.reset(new RedisConnPool(5, host.c_str(), atoi(port.c_str()), pwd.c_str()));
}

RedisManager::~RedisManager() { Close(); }

bool RedisManager::Get(const std::string &key, std::string &value) {
    auto connect = _conn_pool->GetConnection();
    if (nullptr == connect) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connect, "GET %s", key.c_str());
    if (nullptr == reply) {
        LOG(ERROR) << "[GET " << key << "] Failed !";
        _conn_pool->ReturnConnection(connect);
        return false;
    }
    if (reply->type == REDIS_REPLY_STRING) {
        value = reply->str ? reply->str : "";
        LOG(INFO) << "Succeed to execute command [GET " << key << "]";
        freeReplyObject(reply);
        _conn_pool->ReturnConnection(connect);
        return true;
    } else if (reply->type == REDIS_REPLY_NIL) {
        LOG(INFO) << "Key [" << key << "] not found";
        freeReplyObject(reply);
        _conn_pool->ReturnConnection(connect);
        return false;
    } else {
        LOG(ERROR) << "[GET " << key << "] failed ! Reply type: " << reply->type;
        freeReplyObject(reply);
        _conn_pool->ReturnConnection(connect);
        return false;
    }
}

bool RedisManager::Set(const std::string &key, const std::string &value) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connect, "SET %s %s", key.c_str(), value.c_str());
    if (nullptr == reply) {
        LOG(ERROR) << "Execute command [SET " << key << " " << value << "] failed!";
        _conn_pool->ReturnConnection(connect);
        return false;
    }

    bool success = false;
    if (reply->type == REDIS_REPLY_STATUS &&
        (STR_CMP(reply->str, "OK") == 0)) {  // 使用兼容性宏
        success = true;
        LOG(INFO) << "Execute command [SET " << key << " " << value << "] success!";
    } else {
        LOG(ERROR) << "Execute command [SET " << key << " " << value << "] failed!";
    }
    freeReplyObject(reply);
    _conn_pool->ReturnConnection(connect);
    return success;
}

bool RedisManager::Auth(const std::string &password) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connect, "AUTH %s", password.c_str());
    if (reply == nullptr) {
        LOG(ERROR) << "认证失败 - 连接错误";
        _conn_pool->ReturnConnection(connect);
        return false;
    }
    bool success = (reply->type != REDIS_REPLY_ERROR);
    if (success) {
        LOG(INFO) << "认证成功";
    } else {
        LOG(ERROR) << "认证失败: " << (reply->str ? reply->str : "unknown error");
    }
    freeReplyObject(reply);
    _conn_pool->ReturnConnection(connect);
    return success;
}

bool RedisManager::LPush(const std::string &key, const std::string &value) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connect, "LPUSH %s %s", key.c_str(), value.c_str());
    if (nullptr == reply) {
        LOG(ERROR) << "Execute command [LPUSH " << key << " " << value << "] failure !";
        _conn_pool->ReturnConnection(connect);
        return false;
    }
    bool success = (reply->type == REDIS_REPLY_INTEGER && reply->integer > 0);
    if (success) {
        LOG(INFO) << "Execute command [LPUSH " << key << " " << value << "] success !";
    } else {
        LOG(ERROR) << "Execute command [LPUSH " << key << " " << value << "] failure !";
    }
    freeReplyObject(reply);
    _conn_pool->ReturnConnection(connect);
    return success;
}

bool RedisManager::LPop(const std::string &key, std::string &value) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connect, "LPOP %s", key.c_str());
    if (nullptr == reply) {
        LOG(ERROR) << "Execute command [LPOP " << key << "] failure !";
        _conn_pool->ReturnConnection(connect);
        return false;
    }
    if (reply->type == REDIS_REPLY_STRING) {
        value = reply->str ? reply->str : "";
        LOG(INFO) << "Execute command [LPOP " << key << "] success !";
        freeReplyObject(reply);
        _conn_pool->ReturnConnection(connect);
        return true;
    } else if (reply->type == REDIS_REPLY_NIL) {
        LOG(INFO) << "Execute command [LPOP " << key << "] - list is empty";
        freeReplyObject(reply);
        _conn_pool->ReturnConnection(connect);
        return false;
    } else {
        LOG(ERROR) << "Execute command [LPOP " << key << "] failure !";
        freeReplyObject(reply);
        _conn_pool->ReturnConnection(connect);
        return false;
    }
}

bool RedisManager::RPush(const std::string &key, const std::string &value) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connect, "RPUSH %s %s", key.c_str(), value.c_str());
    if (nullptr == reply) {
        LOG(ERROR) << "Execute command [RPUSH " << key << "  " << value << "] failure !";
        _conn_pool->ReturnConnection(connect);
        return false;
    }
    bool success = (reply->type == REDIS_REPLY_INTEGER && reply->integer > 0);
    if (success) {
        LOG(INFO) << "Execute command [RPUSH " << key << "  " << value << "] success !";
    } else {
        LOG(ERROR) << "Execute command [RPUSH " << key << "  " << value << "] failure !";
    }
    freeReplyObject(reply);
    _conn_pool->ReturnConnection(connect);
    return success;
}

bool RedisManager::RPop(const std::string &key, std::string &value) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connect, "RPOP %s", key.c_str());
    if (nullptr == reply) {
        LOG(ERROR) << "Execute command [RPOP " << key << "] failure !";
        _conn_pool->ReturnConnection(connect);
        return false;
    }
    if (reply->type == REDIS_REPLY_STRING) {
        value = reply->str ? reply->str : "";
        LOG(INFO) << "Execute command [RPOP " << key << "] success !";
        freeReplyObject(reply);
        _conn_pool->ReturnConnection(connect);
        return true;
    } else if (reply->type == REDIS_REPLY_NIL) {
        LOG(INFO) << "Execute command [RPOP " << key << "] - list is empty";
        freeReplyObject(reply);
        _conn_pool->ReturnConnection(connect);
        return false;
    } else {
        LOG(ERROR) << "Execute command [RPOP " << key << "] failure !";
        freeReplyObject(reply);
        _conn_pool->ReturnConnection(connect);
        return false;
    }
}

bool RedisManager::HSet(const std::string &key, const std::string &hkey, const std::string &value) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connect, "HSET %s %s %s", key.c_str(), hkey.c_str(), value.c_str());
    if (nullptr == reply) {
        LOG(ERROR) << "Execute command [HSet " << key << "  " << hkey << "  " << value << "] failure !";
        _conn_pool->ReturnConnection(connect);
        return false;
    }
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    if (success) {
        LOG(INFO) << "Execute command [HSet " << key << "  " << hkey << "  " << value << "] success !";
    } else {
        LOG(ERROR) << "Execute command [HSet " << key << "  " << hkey << "  " << value << "] failure !";
    }
    freeReplyObject(reply);
    _conn_pool->ReturnConnection(connect);
    return success;
}

bool RedisManager::HSet(const char *key, const char *hkey, const char *hvalue, size_t hvaluelen) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return false;
    }
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

    auto reply = (redisReply *) redisCommandArgv(connect, 4, argv, argvlen);
    if (nullptr == reply) {
        LOG(ERROR) << "Execute command [HSet " << key << "  " << hkey << "] failure !";
        _conn_pool->ReturnConnection(connect);
        return false;
    }
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    if (success) {
        LOG(INFO) << "Execute command [HSet " << key << "  " << hkey << "] success !";
    } else {
        LOG(ERROR) << "Execute command [HSet " << key << "  " << hkey << "] failure !";
    }
    freeReplyObject(reply);
    _conn_pool->ReturnConnection(connect);
    return success;
}

std::string RedisManager::HGet(const std::string &key, const std::string &hkey) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return "";
    }
    const char *argv[3];
    size_t argvlen[3];
    argv[0] = "HGET";
    argvlen[0] = 4;
    argv[1] = key.c_str();
    argvlen[1] = key.length();
    argv[2] = hkey.c_str();
    argvlen[2] = hkey.length();

    auto reply = (redisReply *) redisCommandArgv(connect, 3, argv, argvlen);
    if (nullptr == reply) {
        LOG(ERROR) << "Execute command [HGet " << key << " " << hkey << "] failure !";
        _conn_pool->ReturnConnection(connect);
        return "";
    }
    std::string value;
    if (reply->type == REDIS_REPLY_STRING) {
        value = reply->str ? reply->str : "";
        LOG(INFO) << "Execute command [HGet " << key << " " << hkey << "] success !";
    } else if (reply->type == REDIS_REPLY_NIL) {
        LOG(INFO) << "Execute command [HGet " << key << " " << hkey << "] - field not found";
    } else {
        LOG(ERROR) << "Execute command [HGet " << key << " " << hkey << "] failure !";
    }
    freeReplyObject(reply);
    _conn_pool->ReturnConnection(connect);
    return value;
}

bool RedisManager::Del(const std::string &key) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connect, "DEL %s", key.c_str());
    if (nullptr == reply) {
        LOG(ERROR) << "Execute command [Del " << key << "] failure !";
        _conn_pool->ReturnConnection(connect);
        return false;
    }
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    if (success) {
        LOG(INFO) << "Execute command [Del " << key << "] success ! Deleted " << reply->integer << " key(s)";
    } else {
        LOG(ERROR) << "Execute command [Del " << key << "] failure !";
    }
    freeReplyObject(reply);
    _conn_pool->ReturnConnection(connect);
    return success;
}

bool RedisManager::ExistsKey(const std::string &key) {
    auto connect = _conn_pool->GetConnection();
    if (connect == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connect, "EXISTS %s", key.c_str());
    if (nullptr == reply) {
        LOG(ERROR) << "Execute command [EXISTS " << key << "] failure !";
        _conn_pool->ReturnConnection(connect);
        return false;
    }
    bool exists = (reply->type == REDIS_REPLY_INTEGER && reply->integer > 0);
    if (exists) {
        LOG(INFO) << "Found [Key " << key << "] exists !";
    } else {
        LOG(INFO) << "Not Found [Key " << key << "] !";
    }
    freeReplyObject(reply);
    _conn_pool->ReturnConnection(connect);
    return exists;
}

void RedisManager::Close() {
    _conn_pool->Close();
}