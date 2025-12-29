#include "Const.h"
#include "ConfigManager.h"
#include "StatusServiceImpl.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

std::string generate_unique_string() {
    // 创建 uuid 对象
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    // 将 uuid 转换为字符串
    std::string unique_string = to_string(uuid);
    return unique_string;
}

Status StatusServiceImpl::GetChatServer(grpc::ServerContext *context, const message::GetChatServerRequest *request,
                                        message::GetChatServerResponse *reply) {
    std::string prefix("status server has received :  ");
    const auto &server = getChatServer();
    reply->set_host(server.host);
    reply->set_port(server.port);
    reply->set_error(ErrorCodes::Success);
    reply->set_token(generate_unique_string());
    insertToken(request->uid(), reply->token());
    return Status::OK;
}

StatusServiceImpl::StatusServiceImpl() {
    auto &cfg = ConfigManager::Inst();
    ChatServer server;
    server.port = cfg["ChatServerA"]["Port"];
    server.host = cfg["ChatServerA"]["Host"];
    server.con_count = 0;
    server.name = cfg["ChatServerA"]["Name"];
    _servers[server.name] = server;

    server.port = cfg["ChatServerB"]["Port"];
    server.host = cfg["ChatServerB"]["Host"];
    server.name = cfg["ChatServerB"]["Name"];
    server.con_count = 0;
    _servers[server.name] = server;
}

ChatServer StatusServiceImpl::getChatServer() {
    std::lock_guard<std::mutex> guard(_server_mtx);
    auto minServer = _servers.begin()->second;
    // 使用 for 循环，找出连接数最少的聊天服务器
    for (const auto &server: _servers) {
        if (server.second.con_count < minServer.con_count) {
            minServer = server.second;
        }
    }
    return minServer;
}

Status StatusServiceImpl::Login(grpc::ServerContext *context,
                                const message::LoginRequest *request, message::LoginResponse *reply) {
    auto uid = request->uid();
    auto token = request->token();
    std::lock_guard<std::mutex> guard(_token_mtx);
    auto iter = _tokens.find(uid);
    if (_tokens.end() == iter) {
        reply->set_error(ErrorCodes::UidInvalid);
        return Status::OK;
    }
    if (iter->second != token) {
        reply->set_error(ErrorCodes::TokenInvalid);
        return Status::OK;
    }
    reply->set_error(ErrorCodes::Success);
    reply->set_uid(uid);
    reply->set_token(token);
    return Status::OK;
}

void StatusServiceImpl::insertToken(int uid, std::string token) {
    std::lock_guard<std::mutex> guard(_token_mtx);
    _tokens[uid] = token;
}