#include "HttpConnection.h"

HttpConnection::HttpConnection(tcp::socket socket) : _socket(std::move(socket)) {}

void HttpConnection::Start() {
    auto self = shared_from_this();
    http::async_read(_socket, _buffer, _request, [self](beast::error_code error, std::size_t bytes_transferred) {
        try {
            if (error) {
                std::cout << "http read error is: " << error.what() << std::endl;
                return;
            }
            boost::ignore_unused(bytes_transferred);
            self->handlerRequest();
            self->checkDeadline();
        }
        catch (std::exception &exp) {
            std::cout << "exception is: " << exp.what() << std::endl;
        }
    });
}

void HttpConnection::handlerRequest() {
    // 设置版本
    _response.version(_request.version());
    // 设置为短连接
    _response.keep_alive(false);

    // 处理 http get 请求
    if (http::verb::get == _request.method()) {
        bool success = LogicSystem::GetInstance()->HandlerGet(_request.target(), shared_from_this());
        // 处理错误情况
        if (!success) {
            _response.result(http::status::not_found);
            _response.set(http::field::content_type, "text/plain");
            beast::ostream(_response.body()) << "url not found.\r\n";
            writeResponse();
            return;
        }

        // 处理正确情况
        _response.result(http::status::ok);
        _response.set(http::field::server, "GateServer");
        writeResponse();
        return;
    }
}

void HttpConnection::writeResponse() {
    auto self = shared_from_this();
    _response.content_length(_response.body().size());
    http::async_write(_socket, _response, [self](beast::error_code error, std::size_t bytes_transferred) {
        // 关闭发送端
        self->_socket.shutdown(tcp::socket::shutdown_send, error);
        self->deadline_.cancel();
    });
}

void HttpConnection::checkDeadline() {
    auto self = shared_from_this();
    deadline_.async_wait([self](beast::error_code error) {
        // 客户端已经超时，则关闭即可
        if (!error) {
            self->_socket.close();
        }
    });
}