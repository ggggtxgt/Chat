#ifndef GATESERVER_GLOG_H
#define GATESERVER_GLOG_H

/* Windows 平台使用，一定需要添加 */
/* 特别注意：这两个宏必须位于 #include <glog/logging.h> 之上 */
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GLOG_USE_GLOG_EXPORT

#include "Singleton.h"

#include <glog/logging.h>

class Glog : public Singleton<Glog> {
public:
    friend class Singleton<Glog>;    // 使用友元
    ~Glog();                         // 析构函数
    void InitLog(const char *name);  // 初始化 glog 属性(外部接口)

private:
    Glog();                          // 构造波函数
    bool _initialized = false;       // 标记是否被初始化
    void initGlog(const char *name); // 实际初始化
};


#endif //GATESERVER_GLOG_H
