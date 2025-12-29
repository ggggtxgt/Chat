#ifndef GATESERVER_STATUSGRPCCLIENT_H
#define GATESERVER_STATUSGRPCCLIENT_H

#include <queue>

#include <grpcpp/grpcpp.h>

#include "Const.h"
#include "Singleton.h"
#include "ConfigManager.h"
#include "../grpc/message.pb.h"
#include "../grpc/message.grpc.pb.h"

using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;

using message::StatusService;
using message::GetChatServerRequest;
using message::GetChatServerResponse;

class StatusConPool {
public:
    StatusConPool(std::size_t size, std::string host, std::string port);    // 构造函数
    ~StatusConPool();                                                       // 析构函数
    std::unique_ptr<StatusService::Stub> GetConnection();                   // 获取连接
    void ReturnConnection(std::unique_ptr<StatusService::Stub> con);        // 归还连接
    void Close();                                                           // 关闭 StatusConPool

private:
    std::size_t size_;
    std::string host_;
    std::string port_;
    std::mutex mutex_;
    std::atomic<bool> b_stop_;
    std::condition_variable cond_;
    std::queue<std::unique_ptr<StatusService::Stub>> connections_;
};

class StatusGrpcClient : public Singleton<StatusGrpcClient> {
    friend class Singleton<StatusGrpcClient>;

public:
    ~StatusGrpcClient() = default;                                  // 使用默认析构函数
    GetChatServerResponse GetChatServer(int uid);                   // 获取 uid 获取服务器
    message::LoginResponse Login(int uid, std::string token);       // 处理登录请求

private:
    StatusGrpcClient();

    std::unique_ptr<StatusConPool> pool_;
};


#endif //GATESERVER_STATUSGRPCCLIENT_H
