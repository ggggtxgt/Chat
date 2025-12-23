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

    RegisterPost("/get_varify_code", [](std::shared_ptr<HttpConnection> connection) {
        auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
        connection->_response.set(http::field::content_type, "text/json");

        Json::Value root;
        Json::Reader reader;
        Json::Value src_root;
        bool parse_success = reader.parse(body_str, src_root);
        if (!parse_success) {
            std::cout << "Failed to parse Json data!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string str = root.toStyledString();
            beast::ostream(connection->_response.body()) << str;
            return true;
        }
        // key 值不存在
        if (!src_root.isMember("email")) {
            std::cout << "Failed to parse Json data!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string str = root.toStyledString();
            beast::ostream(connection->_response.body()) << str;
            return true;
        }
        auto email = src_root["email"].asString();
        std::cout << "email is: " << email << std::endl;
        root["error"] = 0;
        root["email"] = src_root["email"];
        std::string str = root.toStyledString();
        beast::ostream(connection->_response.body()) << str;
        return true;
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

void LogicSystem::RegisterPost(std::string url, HttpHandler handler) {
    _post_handlers.insert(std::make_pair(url, handler));
}

bool LogicSystem::HandlePost(std::string url, std::shared_ptr<HttpConnection> connection) {
    // 没有找到，即出现错误
    if (_post_handlers.find(url) == _post_handlers.end()) {
        return false;
    }
    // 正常情况，处理即可
    _post_handlers[url](connection);
    return true;
}