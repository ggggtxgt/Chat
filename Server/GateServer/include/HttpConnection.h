#ifndef GATESERVER_HTTPCONNECTION_H
#define GATESERVER_HTTPCONNECTION_H

#include <unordered_map>

#include "Const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
    friend class LogicSystem;

    HttpConnection(tcp::socket socket);

    void Start();

private:
    void checkDeadline();               // 超时检测
    void writeResponse();               // 回复应答
    void handleRequest();               // 处理请求
    void preparseGetParam();            // 解析 get 请求参数

private:
    tcp::socket _socket;                                        // 无默认构造，拷贝构造（使用移动构造）
    beast::flat_buffer _buffer{8192};                      // 存储对方发送的数据
    http::request<http::dynamic_body> _request;                 // 存储 HTTP 请求的所有信息
    http::response<http::dynamic_body> _response;               // 构建和存储 HTTP 响应信息
    // 定时器
    net::steady_timer deadline_{_socket.get_executor(), std::chrono::seconds(60)};

    std::string _get_url;                                       // 存储 get 请求的url
    std::unordered_map<std::string, std::string> _get_params;   // 带有参数的 get 请求的映射表
};


#endif //GATESERVER_HTTPCONNECTION_H
