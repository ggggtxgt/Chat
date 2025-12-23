/* Windows 平台使用，一定需要添加 */
/* 特别注意：这两个宏必须位于 #include <glog/logging.h> 之上 */
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GLOG_USE_GLOG_EXPORT

#include <iostream>
#include <glog/logging.h>

#include "CServer.h"

int main(int argc, char *argv[]) {
    // 初始化glog（必须第一个调用）
    google::InitGoogleLogging(argv[0]);
    // 基本配置
    FLAGS_logtostderr = 1;           // 日志同时输出到控制台
    FLAGS_colorlogtostderr = true;   // 控制台彩色输出
    FLAGS_stderrthreshold = 0;       // INFO 及以上级别都输出到控制台
    FLAGS_logbufsecs = 0;            // 日志立即刷新
    LOG(INFO) << "Glog初始化完成";
    LOG(INFO) << "this is formation.";
    LOG(INFO) << "程序即将退出";
    google::ShutdownGoogleLogging();

    try {
        // 指定端口号
        unsigned short port = static_cast<unsigned short>(8080);
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
        ioc.run();
    }
    catch (std::exception &exp) {
        std::cerr << "Error: " << exp.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}