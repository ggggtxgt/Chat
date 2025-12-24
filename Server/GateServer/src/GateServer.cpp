#include <iostream>

#include <hiredis.h>

#include "Glog.h"
#include "CServer.h"
#include "ConfigManager.h"

void TestRedis() {
    // 连接 redis，需要启动才可以进行连接
    // redis 默认监听端口，可以再配置文件中修改
    redisContext *c = redisConnect("127.0.0.1", 6380);
    if (c->err) {
        LOG(ERROR) << "Connect to redisServer faile: n" << c->errstr;
        redisFree(c);
        return;
    }
    LOG(INFO) << "Connect to redisServer Success";

    std::string redis_password = "123456";
    redisReply *r = (redisReply *) redisCommand(c, "AUTH %s", redis_password.c_str());
    if (r->type == REDIS_REPLY_ERROR) {
        LOG(INFO) << "Redis认证失败！";
    } else {
        LOG(INFO) << "Redis认证成功！";
    }
    // 为redis设置key
    const char *command1 = "set stest1 value1";
    // 执行redis命令行
    r = (redisReply *) redisCommand(c, command1);
    // 如果返回NULL则说明执行失败
    if (nullptr == r) {
        LOG(ERROR) << "Execute command1 failure.";
        redisFree(c);
        return;
    }
    // 如果执行失败则释放连接
    if (!(r->type == REDIS_REPLY_STATUS && (strcmp(r->str, "OK") == 0 || strcmp(r->str, "ok") == 0))) {
        LOG(INFO) << "Failed to execute command[" << command1 << "]";
        freeReplyObject(r);
        redisFree(c);
        return;
    }

    // 执行成功 释放redisCommand执行后返回的redisReply所占用的内存
    freeReplyObject(r);
    LOG(INFO) << "Succeed to execute command[" << command1 << "]";
    const char *command2 = "strlen stest1";
    r = (redisReply *) redisCommand(c, command2);

    // 如果返回类型不是整形 则释放连接
    if (r->type != REDIS_REPLY_INTEGER) {
        LOG(ERROR) << "Failed to execute command[" << command2 << "]";
        freeReplyObject(r);
        redisFree(c);
        return;
    }

    // 获取字符串长度
    int length = r->integer;
    freeReplyObject(r);
    LOG(INFO) << "The length of 'stest1' is: " << length;
    LOG(INFO) << "Succeed to execute command[" << command2 << "]";

    // 获取redis键值对信息
    const char *command3 = "get stest1";
    r = (redisReply *) redisCommand(c, command3);
    if (r->type != REDIS_REPLY_STRING) {
        LOG(ERROR) << "Failed to execute command[" << command3 << "]";
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    LOG(INFO) << "The value of 'stest1' is: " << r->str;
    freeReplyObject(r);
    LOG(INFO) << "Succeed to execute command[" << command3 << "]";

    const char *command4 = "get stest2";
    r = (redisReply *) redisCommand(c, command4);
    if (r->type != REDIS_REPLY_NIL) {
        LOG(INFO) << "Failed to execute command[" << command4 << "]";
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    freeReplyObject(r);
    LOG(INFO) << "Succeed to execute command[" << command4 << "]";

    // 释放连接资源
    redisFree(c);
}

int main(int argc, char *argv[]) {
    Glog::GetInstance()->InitLog(argv[0]);
    auto &fCfgMgr = ConfigManager::Inst();
    std::string port_str = fCfgMgr["GateServer"]["Port"];

    TestRedis();

    unsigned short port = atoi(port_str.c_str());
    try {
        // 初始化上下文，底层一个线程运行
        net::io_context ioc{1};
        // 生成信号集
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        // 信号集异步等待
        signals.async_wait([&ioc](const boost::system::error_code &error, int signal_number) {
            // 出现错误，则停止 ioc
            if (error) { return; }
            ioc.stop();
        });
        // 启动 Server 和 ioc
        std::make_shared<CServer>(ioc, port)->Start();
        LOG(INFO) << "GateServer listen on port: " << port;
        ioc.run();
    }
    catch (std::exception &exp) {
        LOG(ERROR) << "Exception Error: " << exp.what();
        return EXIT_FAILURE;
    }

    return 0;
}