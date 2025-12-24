#ifndef GATESERVER_VERIFYGRPCCLIENT_H
#define GATESERVER_VERIFYGRPCCLIENT_H

#include <queue>

#include "const.h"
#include "Singleton.h"
#include "message.grpc.pb.h"

#include "grpcpp/grpcpp.h"

using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;

using message::VarifyService;
using message::GetVarifyRequest;
using message::GetVarifyResponse;

class RPCConnPool {
public:
    RPCConnPool(std::size_t, std::string, std::string);             // 构造函数
    ~RPCConnPool();                                                 // 析构函数
    void Close();                                                   // 关闭 RPCConnPool
    std::unique_ptr<VarifyService::Stub> GetConnection();           // 获取 VarifyService 连接
    void ReturnConnection(std::unique_ptr<VarifyService::Stub>);    // 返回 VarifyService 连接

private:
    std::size_t size_;                  // 队列的容量大小
    std::string host_;                  // RPC 连接的 host
    std::string port_;                  // RCP 连接的 port
    std::mutex mutex_;                  // 互斥量
    std::atomic<bool> b_stop_;          // 标记 RPCConnPool 是否关闭
    std::condition_variable cond_;      // 条件变量

    // @todo 使用锁使得队列线程安全（优化：可以并发编程中的两把锁队列）
    std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
};

class VerifyGrpcClient : public Singleton<VerifyGrpcClient> {
    friend class Singleton<VerifyGrpcClient>;

public:
    GetVarifyResponse GetVarifyCode(std::string email);

private:
    VerifyGrpcClient();

    std::unique_ptr<RPCConnPool> pool_;
};


#endif //GATESERVER_VERIFYGRPCCLIENT_H
