#ifndef GATESERVER_HTTPCONNECTION_H
#define GATESERVER_HTTPCONNECTION_H

#include "Const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
    friend class LogicSystem;

    HttpConnection(tcp::socket socket);

    void Start();

private:
    void checkDeadline();               // 超时检测
    void writeResponse();               // 回复应答
    void handlerRequest();              // 处理请求

private:
    tcp::socket _socket;                                 // 无默认构造，拷贝构造（使用移动构造）
    beast::flat_buffer _buffer{8192};               // 存储对方发送的数据
    http::request<http::dynamic_body> _request;
    http::response<http::dynamic_body> _response;
    // 定时器
    net::steady_timer deadline_{_socket.get_executor(), std::chrono::seconds(60)};
};


#endif //GATESERVER_HTTPCONNECTION_H
