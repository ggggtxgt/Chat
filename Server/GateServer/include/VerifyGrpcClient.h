#ifndef GATESERVER_VERIFYGRPCCLIENT_H
#define GATESERVER_VERIFYGRPCCLIENT_H

#include "Const.h"
#include "Singleton.h"
#include "message.grpc.pb.h"

#include "grpcpp/grpcpp.h"

using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;

using message::VarifyService;
using message::GetVarifyRequest;
using message::GetVarifyResponse;

class VerifyGrpcClient : public Singleton<VerifyGrpcClient> {
    friend class Singleton<VerifyGrpcClient>;

public:
    GetVarifyResponse GetVarifyCode(std::string email);

private:
    VerifyGrpcClient();

    std::unique_ptr<VarifyService::Stub> stub_;
};


#endif //GATESERVER_VERIFYGRPCCLIENT_H
