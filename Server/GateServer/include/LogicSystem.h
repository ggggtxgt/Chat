#ifndef GATESERVER_LOGICSYSTEM_H
#define GATESERVER_LOGICSYSTEM_H

#include "Const.h"

class HttpConnection;

typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;

class LogicSystem : public Singleton<LogicSystem> {
public:
    friend class Singleton<LogicSystem>;

    ~LogicSystem();

    bool HandleGet(std::string, std::shared_ptr<HttpConnection>);   // 处理 get 请求
    void RegisterGet(std::string, HttpHandler);                     // 注册 get 请求

private:
    LogicSystem();

    std::map<std::string, HttpHandler> _get_handlers;               // 处理 get 请求的集合
    std::map<std::string, HttpHandler> _post_handlers;              // 处理 post 请求的集合

};


#endif //GATESERVER_LOGICSYSTEM_H
