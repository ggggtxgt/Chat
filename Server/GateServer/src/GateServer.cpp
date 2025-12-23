/* Windows 平台使用，一定需要添加 */
/* 特别注意：这两个宏必须位于 #include <glog/logging.h> 之上 */
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GLOG_USE_GLOG_EXPORT

#include <iostream>
#include <glog/logging.h>

int main(int argc, char* argv[]) {
    // 初始化glog（必须第一个调用）
    google::InitGoogleLogging(argv[0]);

    // 基本配置
    FLAGS_logtostderr = 1;           // 日志同时输出到控制台
    FLAGS_colorlogtostderr = true;   // 控制台彩色输出
    FLAGS_stderrthreshold = 0;       // INFO 及以上级别都输出到控制台
    FLAGS_logbufsecs = 0;            // 日志立即刷新

    LOG(INFO) << "Glog初始化完成";
    LOG(INFO) << "this is formation.";

    std::cout << "Hello, World!" << std::endl;

    LOG(INFO) << "程序即将退出";
    google::ShutdownGoogleLogging();

    return 0;
}