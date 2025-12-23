#include "LogicSystem.h"
#include "HttpConnection.h"

LogicSystem::LogicSystem() {
    RegisterGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
        beast::ostream(connection->_response.body()) << "receive get_test request.";
    });
}

LogicSystem::~LogicSystem() {}

void LogicSystem::RegisterGet(std::string url, HttpHandler handler) {
    _get_handlers.insert(make_pair(url, handler));
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> connection) {
    // 没有找到，即出现错误
    if (_get_handlers.find(path) == _get_handlers.end()) {
        return false;
    }
    // 正常情况，处理即可
    _get_handlers[path](connection);
    return true;
}