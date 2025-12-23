#ifndef GATESERVER_CONST_H
#define GATESERVER_CONST_H

#include <map>
#include <memory>
#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>

#include "Singleton.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>#include <boost/beast/http.hpp>

enum ErrorCodes {
    Success = 0,                        // 成功
    Error_Json = 1001,                  // JSON 解析错误
    RPCFailed = 1002,                   // RPC 连接失败
};

#endif //GATESERVER_CONST_H
