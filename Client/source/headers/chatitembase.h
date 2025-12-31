/******************************************************************************
 * @file       chatitembase.h
 * @brief      聊天信息项的头文件
 * @author     ggggtxgt
 * @date       2025/12/31 15:53
 * @history
*****************************************************************************/
#ifndef CLIENT_CHATITEMBASE_H
#define CLIENT_CHATITEMBASE_H

#include <QLabel>
#include <QWidget>
#include <QGridLayout>

#include "global.h"

class ChatItemBase : public QWidget {
Q_OBJECT

public:
    explicit ChatItemBase(ChatRole role, QWidget *parent = nullptr);            // 构造函数
    ~ChatItemBase() override;                                                   // 析构函数
    void SetUserName(const QString &name);                                      // 设置用户昵称
    void SetUserIcon(const QPixmap &icon);                                      // 设置用户头像
    void SetWidget(QWidget *widget);                                            // 设置气泡窗口

private:
    ChatRole _role;
    QLabel *_name_label;
    QLabel *_icon_label;
    QWidget *_bubble_widget;
};


#endif //CLIENT_CHATITEMBASE_H
