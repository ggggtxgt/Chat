#ifndef GATESERVER_CSERVER_H
#define GATESERVER_CSERVER_H

#include "Const.h"

class CServer : public std::enable_shared_from_this<CServer> {
public:
    CServer(boost::asio::io_context &ioc, unsigned short &port);

    void Start();

private:
    tcp::socket _socket;
    tcp::acceptor _acceptor;
    net::io_context &_io_context;
};


#endif //GATESERVER_CSERVER_H
