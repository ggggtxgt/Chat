#include "global.h"
#include "tcpmanager.h"
#include "usermanager.h"
#include "listitembase.h"
#include "contactuserlist.h"

#include <QTimer>
#include <QCoreApplication>
#include <QRandomGenerator>

ContactUserList::ContactUserList(QWidget *parent) : _add_friend_item(nullptr), _load_pending(false) {
    Q_UNUSED(parent)
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
    this->viewport()->installEventFilter(this);

    // 模拟从数据库或者后端传输过来的数据,进行列表加载
    addContactUserList();
    // 连接点击的信号和槽
    connect(this, &QListWidget::itemClicked, this, &ContactUserList::slot_item_clicked);
//    // 链接对端同意认证后通知的信号
//    connect(TcpManager::GetInstance().get(), &TcpManager::signal_add_auth_friend,
//            this, &ContactUserList::slot_add_auth_firend);
//
//    // 链接自己点击同意认证后界面刷新
//    connect(TcpManager::GetInstance().get(), &TcpManager::sig_auth_rsp,
//            this, &ContactUserList::slot_auth_rsp);
}

