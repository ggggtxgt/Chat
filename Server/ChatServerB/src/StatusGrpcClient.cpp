#include "StatusGrpcClient.h"

StatusConPool::StatusConPool(std::size_t size, std::string host, std::string port)
        : size_(size), host_(host), port_(port), b_stop_(false) {
    for (int i = 0; i < size_; i++) {
        std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port,
                                                               grpc::InsecureChannelCredentials());
        connections_.push(StatusService::NewStub(channel));
    }
}

StatusConPool::~StatusConPool() {
    std::lock_guard<std::mutex> lock(mutex_);
    Close();
    while (!connections_.empty()) {
        connections_.pop();
    }
}

std::unique_ptr<StatusService::Stub> StatusConPool::GetConnection() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this] {
        if (b_stop_) { return true; }
        return !connections_.empty();
    });
    auto con = std::move(connections_.front());
    connections_.pop();
    return con;
}

void StatusConPool::ReturnConnection(std::unique_ptr<StatusService::Stub> con) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (b_stop_) return;
    connections_.push(std::move(con));
    cond_.notify_one();
}

void StatusConPool::Close() {
    b_stop_ = true;
    cond_.notify_all();
}

GetChatServerResponse StatusGrpcClient::GetChatServer(int uid) {
    ClientContext context;
    GetChatServerResponse reply;
    GetChatServerRequest request;
    request.set_uid(uid);

    auto stub = pool_->GetConnection();
    Status status = stub->GetChatServer(&context, request, &reply);
    Defer defer([&stub, this]() { pool_->ReturnConnection(std::move(stub)); });

    if (status.ok()) { return reply; }
    else {
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}

message::LoginResponse StatusGrpcClient::Login(int uid, std::string token) {
    ClientContext context;
    message::LoginResponse reply;
    message::LoginRequest request;
    request.set_uid(uid);
    request.set_token(token);

    auto stub = pool_->GetConnection();
    Status status = stub->Login(&context, request, &reply);
    Defer defer([&stub, this]() {
        pool_->ReturnConnection(std::move(stub));
    });
    if (status.ok()) {
        return reply;
    } else {
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}

StatusGrpcClient::StatusGrpcClient() {
    auto &gCfgMgr = ConfigManager::Inst();
    std::string host = gCfgMgr["StatusServer"]["Host"];
    std::string port = gCfgMgr["StatusServer"]["Port"];
    pool_.reset(new StatusConPool(5, host, port));
}