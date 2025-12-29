#ifndef GATESERVER_CSERVER_H
#define GATESERVER_CSERVER_H

#include <map>
#include <mutex>
#include <memory.h>

#include <boost/asio.hpp>

#include "CSession.h"

using boost::asio::ip::tcp;

class CServer {
public:
    ~CServer();                                                         // 析构函数
    CServer(boost::asio::io_context &, short port);                     // 构造函数
    void ClearSession(std::string);                                     // 删除指定会话

private:
    void StartAccept();                                                             // 异步接受客户端连接
    void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code &);// 处理客户端连接请求的回调函数

private:
    short _port;                                                        // 服务器监听的端口号
    std::mutex _mutex;                                                  // 保护会话映射表并发访问
    tcp::acceptor _acceptor;                                            // TCP接受器对象，用于监听和接受客户端连接
    boost::asio::io_context &_io_context;                               // IO服务上下文引用，用于处理异步IO操作
    std::map<std::string, std::shared_ptr<CSession>> _sessions;         // 会话映射表，存储所有活动会话
};


#endif //GATESERVER_CSERVER_H
