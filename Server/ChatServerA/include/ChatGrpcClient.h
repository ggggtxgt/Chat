#ifndef CHATSERVERA_CHATGRPCCLIENT_H
#define CHATSERVERA_CHATGRPCCLIENT_H

#include <queue>
#include <memory>

#include "Const.h"
#include "Singleton.h"
#include "ConfigManager.h"

#include "message.pb.h"
#include "message.grpc.pb.h"

#include "grpcpp/grpcpp.h"

using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;

using message::AddFriendReq;
using message::AddFriendRsp;

using message::LoginRsp;
using message::LoginReq;
using message::ChatService;
using message::GetChatServerRsp;

class ChatConPool {
public:
    ChatConPool(size_t poolSize, std::string host, std::string port);

    ~ChatConPool();

    std::unique_ptr<ChatService::Stub> getConnection();

    void returnConnection(std::unique_ptr<ChatService::Stub> context);

    void Close();

private:
    std::atomic<bool> b_stop_;
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::queue<std::unique_ptr<ChatService::Stub>> connections_;
};


#endif //CHATSERVERA_CHATGRPCCLIENT_H
