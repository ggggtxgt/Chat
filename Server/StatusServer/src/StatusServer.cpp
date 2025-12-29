#include <memory>
#include <string>
#include <thread>
#include <iostream>

#include "Glog.h"
#include "Const.h"
#include "MysqlManager.h"
#include "ConfigManager.h"
#include "StatusServiceImpl.h"

void RunServer() {
    auto &cfg = ConfigManager::Inst();

    std::string server_address(cfg["StatusServer"]["Host"] + ":" + cfg["StatusServer"]["Port"]);
    StatusServiceImpl service;

    grpc::ServerBuilder builder;
    // 监听端口和添加服务
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    // 构建并启动gRPC服务器
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    LOG(INFO) << "Server listening on " << server_address;

    // 创建Boost.Asio的io_context
    boost::asio::io_context io_context;
    // 创建signal_set用于捕获SIGINT
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

    // 设置异步等待SIGINT信号
    signals.async_wait([&server, &io_context](const boost::system::error_code &error, int signal_number) {
        if (!error) {
            LOG(ERROR) << "Shutting down server...";
            server->Shutdown(); // 优雅地关闭服务器
            io_context.stop();  // 停止 io_context
        }
    });

    // 在单独的线程中运行io_context
    std::thread([&io_context]() { io_context.run(); }).detach();

    // 等待服务器关闭
    server->Wait();

}

int main(int argc, char **argv) {
    Glog::GetInstance()->InitLog(argv[0]);
    auto &cfg = ConfigManager::Inst();

    try {
        RunServer();
    }
    catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}