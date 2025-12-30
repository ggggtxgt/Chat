/******************************************************************************
 * @file       chatuserlist.h
 * @brief      自定义用户聊天列表头文件
 * @author     ggggtxgt
 * @date       2025/12/30 16:45
 * @history
*****************************************************************************/
#ifndef CLIENT_CHATUSERLIST_H
#define CLIENT_CHATUSERLIST_H

#include <QDebug>
#include <QEvent>
#include <QScrollBar>
#include <QWheelEvent>
#include <QListWidget>

class ChatUserList : public QListWidget {
Q_OBJECT

public:
    explicit ChatUserList(QWidget *parent = nullptr);                           // 构造函数
    ~ChatUserList() override;                                                   // 析构函数

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;                 // 事件过滤

signals:

    void signal_loading_chat_user();                                            // 加载聊天用户
};


#endif //CLIENT_CHATUSERLIST_H
