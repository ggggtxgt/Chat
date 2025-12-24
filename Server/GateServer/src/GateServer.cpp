#include <iostream>

#include "Glog.h"
#include "CServer.h"
#include "RedisManager.h"
#include "ConfigManager.h"

void TestRedisManager() {
    assert(RedisManager::GetInstance()->Set("blogwebsite", "llfc.club"));
    std::string value = "";
    assert(RedisManager::GetInstance()->Get("blogwebsite", value));
    assert(RedisManager::GetInstance()->Get("nonekey", value) == false);
    assert(RedisManager::GetInstance()->HSet("bloginfo", "blogwebsite", "llfc.club"));
    assert(RedisManager::GetInstance()->HGet("bloginfo", "blogwebsite") != "");
    assert(RedisManager::GetInstance()->ExistsKey("bloginfo"));
    assert(RedisManager::GetInstance()->Del("bloginfo"));
    assert(RedisManager::GetInstance()->Del("bloginfo"));
    assert(RedisManager::GetInstance()->ExistsKey("bloginfo") == false);
    assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue1"));
    assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue2"));
    assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue3"));
    assert(RedisManager::GetInstance()->RPop("lpushkey1", value));
    assert(RedisManager::GetInstance()->RPop("lpushkey1", value));
    assert(RedisManager::GetInstance()->LPop("lpushkey1", value));
    assert(RedisManager::GetInstance()->LPop("lpushkey2", value) == false);
    RedisManager::GetInstance()->Close();
}

int main(int argc, char *argv[]) {
    Glog::GetInstance()->InitLog(argv[0]);
    auto &fCfgMgr = ConfigManager::Inst();
    std::string port_str = fCfgMgr["GateServer"]["Port"];

    TestRedisManager();

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