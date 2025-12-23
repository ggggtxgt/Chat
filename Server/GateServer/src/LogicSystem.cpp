#include "LogicSystem.h"
#include "HttpConnection.h"

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

LogicSystem::LogicSystem() {
    RegisterGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
        beast::ostream(connection->_response.body()) << "receive get_test request.\n";
        int i = 0;
        for (auto &elem: connection->_get_params) {
            i++;
            beast::ostream(connection->_response.body())
                    << "param " << i << "  key is " << elem.first << "  value is "
                    << elem.second << std::endl;
        }
    });
}

LogicSystem::~LogicSystem() {}

void LogicSystem::RegisterGet(std::string url, HttpHandler handler) {
    _get_handlers.insert(make_pair(url, handler));
}

bool LogicSystem::HandleGet(std::string url, std::shared_ptr<HttpConnection> connection) {
    // 没有找到，即出现错误
    if (_get_handlers.find(url) == _get_handlers.end()) {
        return false;
    }
    // 正常情况，处理即可
    _get_handlers[url](connection);
    return true;
}