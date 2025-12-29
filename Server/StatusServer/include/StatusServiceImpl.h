#ifndef GATESERVER_STATUSSERVICEIMPL_H
#define GATESERVER_STATUSSERVICEIMPL_H

#include <mutex>

#include "grpcpp/grpcpp.h"

#include "message.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using message::GetChatServerRequest;
using message::GetChatServerResponse;
using message::LoginRequest;
using message::LoginResponse;
using message::StatusService;

// 存储聊天服务器相关信息
struct ChatServer {
    std::string host;
    std::string port;
    std::string name;
    int con_count;
};

class StatusServiceImpl : public StatusService::Service {
public:
    StatusServiceImpl();                                                // 构造函数
    Status GetChatServer(ServerContext *, const GetChatServerRequest *,
                         GetChatServerResponse *) override;             // 获取一个聊天服务器地址
    Status Login(ServerContext *, const LoginRequest *,
                 LoginResponse *) override;                             // 验证用户登录信息

private:
    std::mutex _token_mtx;                                              // 保护 _tokens 映射的互斥锁
    std::mutex _server_mtx;                                             // 保护 _servers 映射的互斥锁
    std::unordered_map<int, std::string> _tokens;                       // 存储 uid 到 token 的映射关系
    std::unordered_map<std::string, ChatServer> _servers;               // 存储可用的聊天服务器信息

private:
    ChatServer getChatServer();                                         // 获取连接数最少的聊天服务器
    void insertToken(int uid, std::string token);                       // 插入 token 信息
};


#endif //GATESERVER_STATUSSERVICEIMPL_H
