#ifndef GATESERVER_MSGNODE_H
#define GATESERVER_MSGNODE_H

#include <string>
#include <iostream>

#include <boost/asio.hpp>

#include "Const.h"

using boost::asio::ip::tcp;

class LogicSystem;

class MsgNode {
public:
    MsgNode(short max_len);                                     // 构造函数
    ~MsgNode();                                                 // 析构函数
    void Clear();                                               //

public:
    char *_data;
    short _cur_len;
    short _total_len;
};

class RecvNode : public MsgNode {
    friend class LogicSystem;

public:
    RecvNode(short max_len, short msg_id);

private:
    short _msg_id;
};

class SendNode : public MsgNode {
    friend class LogicSystem;

public:
    SendNode(const char *msg, short max_len, short msg_id);

private:
    short _msg_id;
};

#endif //GATESERVER_MSGNODE_H
