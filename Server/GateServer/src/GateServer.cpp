#include <iostream>

#include "Glog.h"
#include "CServer.h"
#include "ConfigManager.h"

int main(int argc, char *argv[]) {
    Glog::GetInstance()->InitLog(argv[0]);
    ConfigManager fCfgMgr;
    std::string port_str = fCfgMgr["GateServer"]["Port"];
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