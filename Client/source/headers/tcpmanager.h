/******************************************************************************
 * @file       tcpmanager.h
 * @brief      tcp管理类头文件
 * @author     ggggtxgt
 * @date       2025/12/29 17:16
 * @history
*****************************************************************************/
#ifndef CLIENT_TCPMANAGER_H
#define CLIENT_TCPMANAGER_H

#include "global.h"
#include "userdata.h"
#include "singleton.h"

#include <functional>

#include <QObject>
#include <QTcpSocket>


class TcpManager : public QObject, public Singleton<TcpManager>,
                   public std::enable_shared_from_this<TcpManager> {
Q_OBJECT

public:
    friend class Singleton<TcpManager>;

    ~TcpManager();

private:
    TcpManager();                                                                   // 构造函数
    void initHandlers();                                                            // 初始化处理器（注册回调处理）
    void handleMessage(RequestId id, int len, QByteArray data);                     // 处理数据（调用_handlers保存的函数）

private:
    QString _host;
    uint16_t _port;
    QTcpSocket _socket;
    QByteArray _buffer;
    bool _b_recv_pending;
    quint16 _message_id;
    quint16 _message_len;
    QMap<RequestId, std::function<void(RequestId, int, QByteArray)>> _handlers;

signals:

    void signal_switch_chatdlg();                               // 切换到聊天界面
    void signal_login_failed(int);                              // 登录失败
    void signal_con_success(bool b_success);                    // 连接成功
    void signal_send_data(RequestId id, QString data);          // 发送信息
    void signal_user_search(std::shared_ptr<SearchInfo>);       // 按下确认搜索按钮

public slots:

    void slot_tcp_connect(ServerInfo);                          // 建立 TCP 连接
    void slot_send_data(RequestId id, QString data);            // 发送信息
};


#endif //CLIENT_TCPMANAGER_H
