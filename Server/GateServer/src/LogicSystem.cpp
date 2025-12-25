#include "Glog.h"
#include "LogicSystem.h"
#include "RedisManager.h"
#include "MysqlManager.h"
#include "HttpConnection.h"
#include "VerifyGrpcClient.h"

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
            LOG(ERROR) << "Failed to parse Json data!";
            root["error"] = ErrorCodes::Error_Json;
            std::string str = root.toStyledString();
            beast::ostream(connection->_response.body()) << str;
            return true;
        }
        // key 值不存在
        if (!src_root.isMember("email")) {
            LOG(ERROR) << "Failed to parse Json data!";
            root["error"] = ErrorCodes::Error_Json;
            std::string str = root.toStyledString();
            beast::ostream(connection->_response.body()) << str;
            return true;
        }
        auto email = src_root["email"].asString();
        GetVarifyResponse response = VerifyGrpcClient::GetInstance()->GetVarifyCode(email);
        LOG(INFO) << "email is: " << email;
        root["error"] = response.error();
        root["email"] = src_root["email"];
        std::string str = root.toStyledString();
        beast::ostream(connection->_response.body()) << str;
        return true;
    });

    RegisterPost("/user_register", [](std::shared_ptr<HttpConnection> conn) {
        auto body_str = boost::beast::buffers_to_string(conn->_request.body().data());
        LOG(INFO) << "receive body is: \n" << body_str;
        conn->_response.set(http::field::content_type, "text/json");
        Json::Value root;
        Json::Reader reader;
        Json::Value src_root;
        bool parse_success = reader.parse(body_str, src_root);
        if (!parse_success) {
            LOG(ERROR) << "Failed to parse JSON data !";
            root["root"] = ErrorCodes::Error_Json;
            std::string str = root.toStyledString();
            beast::ostream(conn->_response.body()) << str;
            return true;
        }

        auto email = src_root["email"].asString();
        auto name = src_root["user"].asString();
        auto pwd = src_root["passwd"].asString();
        auto confirm = src_root["confirm"].asString();

        if (pwd != confirm) {
            std::cout << "password err" << std::endl;
            root["error"] = ErrorCodes::PasswordErr;
            std::string jsonstr = root.toStyledString();
            beast::ostream(conn->_response.body()) << jsonstr;
            return true;
        }

        // 先查找redis中email对应的验证码是否合理
        std::string varify_code;
        // 注意需要添加前缀
        bool b_get_varify = RedisManager::GetInstance()->Get(CODEPREFIX + src_root["email"].asString(), varify_code);
        if (!b_get_varify) {
            std::cout << " get varify code expired" << std::endl;
            root["error"] = ErrorCodes::VarifyExpired;
            std::string jsonstr = root.toStyledString();
            beast::ostream(conn->_response.body()) << jsonstr;
            return true;
        }

        // 验证码错误
        if (varify_code != src_root["varifycode"].asString()) {
            std::cout << " varify code error" << std::endl;
            root["error"] = ErrorCodes::VarifyCodeErr;
            std::string jsonstr = root.toStyledString();
            beast::ostream(conn->_response.body()) << jsonstr;
            return true;
        }

        // 访问redis查找
        /*
        bool b_usr_exist = RedisManager::GetInstance()->ExistsKey(src_root["user"].asString());
        if (b_usr_exist) {
            std::cout << " user exist" << std::endl;
            root["error"] = ErrorCodes::UserExist;
            std::string jsonstr = root.toStyledString();
            beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }
        */

        // 查找数据库判断用户是否存在
        int uid = MysqlManager::GetInstance()->RegisterUser(name, email, pwd);
        if (0 == uid || -1 == uid) {
            std::cout << "user or email exist" << std::endl;
            root["error"] = ErrorCodes::UserExist;
            std::string jsonstr = root.toStyledString();
            beast::ostream(conn->_response.body()) << jsonstr;
            return true;
        }
        root["error"] = 0;
        root["uid"] = uid;
        root["email"] = email;
        root["user"] = name;
        root["passwd"] = pwd;
        root["confirm"] = confirm;
        root["varifycode"] = src_root["varifycode"].asString();
        std::string jsonstr = root.toStyledString();
        beast::ostream(conn->_response.body()) << jsonstr;
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