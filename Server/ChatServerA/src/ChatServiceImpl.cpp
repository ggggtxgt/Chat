#include "CSession.h"
#include "UserManager.h"
#include "MysqlManager.h"
#include "ChatServiceImpl.h"

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

ChatServiceImpl::ChatServiceImpl() {}

Status ChatServiceImpl::NotifyAddFriend(ServerContext *context, const AddFriendReq *request, AddFriendRsp *reply) {
    return Status::OK;
}

Status ChatServiceImpl::NotifyAuthFriend(ServerContext *context, const AuthFriendReq *request, AuthFriendRsp *reply) {
    return Status::OK;
}

Status ChatServiceImpl::NotifyTextChatMsg(::grpc::ServerContext *context, const TextChatMsgReq *request,
                                          TextChatMsgRsp *reply) {
    return Status::OK;
}


bool ChatServiceImpl::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo> &userinfo) {
    return true;
}

Status ChatServiceImpl::NotifyKickUser(::grpc::ServerContext *context,
                                       const KickUserReq *request, KickUserRsp *reply) {
    return Status::OK;
}

void ChatServiceImpl::RegisterServer(std::shared_ptr<CServer> pServer) {
    _p_server = pServer;
}