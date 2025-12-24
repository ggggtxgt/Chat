#include "ConfigManager.h"
#include "VerifyGrpcClient.h"

RPCConnPool::RPCConnPool(std::size_t size, std::string host, std::string port) :
        size_(size), host_(host), port_(port) {
    for (std::size_t i = 0; i < size_; i++) {
        std::shared_ptr<Channel> chanel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
        // 为何能将 unique_ptr 的指针加入？拷贝构造？移动构造？
        // 调用移动构造，std::unique_ptr 禁用拷贝构造、拷贝赋值，但提供移动构造、移动赋值
        connections_.push(VarifyService::NewStub(chanel));
    }
}

RPCConnPool::~RPCConnPool() {
    std::lock_guard<std::mutex> lock(mutex_);
    // 通知挂起的线程
    Close();
    while (!connections_.empty()) {
        connections_.pop();
    }
}

void RPCConnPool::Close() {
    b_stop_ = true;
    cond_.notify_all();
}

std::unique_ptr<VarifyService::Stub> RPCConnPool::GetConnection() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this]() {
        // 使用带有谓词的等待：wait(lock, predicate) -- 带有条件的等待
        // 返回true: 立即继续执行，不阻塞；返回false: 开始等待
        if (b_stop_) {
            return true;
        }
        return !connections_.empty();
    });
    if (b_stop_) {
        return nullptr;
    }
    auto context = std::move(connections_.front());
    connections_.pop();
    return context;
}

void RPCConnPool::ReturnConnection(std::unique_ptr<VarifyService::Stub> connection) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (b_stop_) {
        return;
    }
    connections_.push(std::move(connection));
    cond_.notify_one();
}

GetVarifyResponse VerifyGrpcClient::GetVarifyCode(std::string email) {
    ClientContext context;
    GetVarifyRequest request;
    GetVarifyResponse response;
    request.set_email(email);

    auto stub = pool_->GetConnection();
    Status status = stub->GetVarifyCode(&context, request, &response);
    if (status.ok()) {
        pool_->ReturnConnection(std::move(stub));
        return response;
    } else {
        pool_->ReturnConnection(std::move(stub));
        response.set_error(ErrorCodes::RPCFailed);
        return response;
    }
}

VerifyGrpcClient::VerifyGrpcClient() {
    auto &gCfgMgr = ConfigManager::Inst();
    std::string host = gCfgMgr["VarifyServer"]["Host"];
    std::string port = gCfgMgr["VarifyServer"]["Port"];
    pool_.reset(new RPCConnPool(5, host, port));
}