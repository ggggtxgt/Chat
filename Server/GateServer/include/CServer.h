#ifndef GATESERVER_CSERVER_H
#define GATESERVER_CSERVER_H

#include "Const.h"

class CServer : public std::enable_shared_from_this<CServer> {
public:
    CServer(boost::asio::io_context &ioc, unsigned short &port);

    void Start();

private:
    tcp::socket _socket;                // 代表一个 TCP 连接端点，用于数据收发
    tcp::acceptor _acceptor;            // 监听指定端口，接受新的 TCP 连接
};


#endif //GATESERVER_CSERVER_H
