#ifndef GATESERVER_CSESSION_H
#define GATESERVER_CSESSION_H

#include <queue>
#include <mutex>
#include <memory>

#include "boost/asio.hpp"
#include "boost/beast.hpp"
#include "boost/beast/http.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/uuid/uuid_generators.hpp"

#include "Const.h"
#include "MsgNode.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class CServer;

class LogicSystem;

class CSession : public std::enable_shared_from_this<CSession> {
public:
    CSession(boost::asio::io_context &, CServer *);                     // 构造函数
    ~CSession();                                                        // 析构函数
    tcp::socket &GetSocket();                                           // 返回当前会话关联的 TCP socket 引用
    std::string &GetUuid();                                             // 返回当前会话的唯一标识符（UUID）
    void Start();                                                       // 启动会话，开始异步读取消息头部
    void Send(char *msg, short max_length, short msgid);                // 发送消息到客户端--字符数组
    void Send(std::string msg, short msgid);                            // 发送消息到客户端--字符串
    void Close();                                                       // 关闭当前会话的 socket 连接
    std::shared_ptr<CSession> SharedSelf();                             // 返回指向当前会话对象的 shared_ptr（防止提前释放）
    void AsyncReadBody(int length);                                     // 异步读取消息体（数据部分）
    void AsyncReadHead(int total_len);                                  // 异步读取消息头部（固定长度）

private:
    // 异步读取指定长度的完整数据，完成后调用回调函数
    void asyncReadFull(std::size_t, std::function<void(const boost::system::error_code &, std::size_t)>);

    // 辅助函数，用于递归读取数据直到达到指定长度
    void asyncReadLen(std::size_t, std::size_t, std::function<void(const boost::system::error_code &, std::size_t)>);

    // 异步写操作完成后的回调函数：发送成功--继续发送队列中的下一条消息；否则关闭连接
    void HandleWrite(const boost::system::error_code &error, std::shared_ptr<CSession> shared_self);

private:
    bool _b_close;                                      // 标记会话是否已经关闭
    CServer *_server;                                   // 指向所属服务器的指针，用于会话管理和清理
    std::string _uuid;                                  // 会话的唯一标识符（UUID 字符串）
    bool _b_head_parse;                                 // 标记是否已经解析了消息头部
    tcp::socket _socket;                                // boost中的socket对象
    std::mutex _send_lock;                              // 发送队列的互斥锁
    char _data[MAX_LENGTH];                             // 接收数据的缓冲区
    std::shared_ptr<MsgNode> _recv_head_node;           // 收到的头部结构
    std::shared_ptr<RecvNode> _recv_msg_node;           // 收到的消息结构
    std::queue<std::shared_ptr<SendNode>> _send_que;    // 发送队列
};

class LogicNode {
    friend class LogicSystem;

public:
    LogicNode(std::shared_ptr<CSession>, std::shared_ptr<RecvNode>);

private:
    std::shared_ptr<CSession> _session;                 // 发送消息的会话对象
    std::shared_ptr<RecvNode> _recv_node;               // 接收到的消息节点
};

#endif //GATESERVER_CSESSION_H
