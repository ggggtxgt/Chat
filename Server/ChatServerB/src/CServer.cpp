#include <iostream>

#include "Glog.h"
#include "CServer.h"
#include "AsioIOServicePool.h"

CServer::CServer(boost::asio::io_context &io_context, short port) : _io_context(io_context), _port(port),
                                                                    _acceptor(io_context,
                                                                              tcp::endpoint(tcp::v4(), port)) {
    LOG(INFO) << "Server start success, listen on port: " << _port;
    StartAccept();
}

CServer::~CServer() { LOG(INFO) << "Server destruct listen on port: " << _port; }

void CServer::HandleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code &error) {
    if (!error) {
        new_session->Start();
        std::lock_guard<std::mutex> lock(_mutex);
        _sessions.insert(std::make_pair(new_session->GetUuid(), new_session));
    } else {
        LOG(ERROR) << "session accept failed, error is: " << error.what();
    }
    StartAccept();
}

void CServer::StartAccept() {
    auto &io_context = AsioIOServicePool::GetInstance()->GetIOService();
    std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_context, this);
    _acceptor.async_accept(new_session->GetSocket(),
                           std::bind(&CServer::HandleAccept, this, new_session, std::placeholders::_1));
}

void CServer::ClearSession(std::string uuid) {
    std::lock_guard<std::mutex> lock(_mutex);
    _sessions.erase(uuid);
}