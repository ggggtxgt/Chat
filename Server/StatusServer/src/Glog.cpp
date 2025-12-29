#include "Glog.h"

Glog::Glog() : _initialized(false) {}

Glog::~Glog() { google::ShutdownGoogleLogging(); }

void Glog::InitLog(const char *name) {
    if (!_initialized) {
        initGlog(name);
        _initialized = true;
    }
}

void Glog::initGlog(const char *program_name) {
    // 必须先初始化 glog
    google::InitGoogleLogging(program_name);

    FLAGS_logtostderr = 1;           // 日志同时输出到控制台
    FLAGS_colorlogtostderr = true;   // 控制台彩色输出
    FLAGS_stderrthreshold = 0;       // INFO 及以上级别都输出到控制台
    FLAGS_logbufsecs = 0;            // 日志立即刷新

    LOG(INFO) << "Glog初始化完成";
}