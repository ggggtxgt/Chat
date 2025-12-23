#include "VerifyGrpcClient.h"

GetVarifyResponse VerifyGrpcClient::GetVarifyCode(std::string email) {
    ClientContext context;
    GetVarifyRequest request;
    GetVarifyResponse response;
    request.set_email(email);

    Status status = stub_->GetVarifyCode(&context, request, &response);
    if (status.ok()) {
        return response;
    } else {
        response.set_error(ErrorCodes::RPCFailed);
        return response;
    }
}

VerifyGrpcClient::VerifyGrpcClient() {
    std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051",
                                                           grpc::InsecureChannelCredentials());
    stub_ = VarifyService::NewStub(channel);
}