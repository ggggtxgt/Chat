/******************************************************************************
 * @file       chatdialog.h
 * @brief      聊天界面头文件
 * @author     ggggtxgt
 * @date       2025/12/30 13:08
 * @history
*****************************************************************************/
#ifndef CLIENT_CHATDIALOG_H
#define CLIENT_CHATDIALOG_H

#include <QDialog>

#include "global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChatDialog; }
QT_END_NAMESPACE

class ChatDialog : public QDialog {
Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);                         // 构造函数
    ~ChatDialog() override;                                                 // 析构函数
    void AddChatUserList();                                                 // 添加聊天界面用户列表

private:
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;

private:
    void showSearch(bool b_search = false);                                 // 显示各种列表

private slots:

    void slot_loading_chat_user();                                          // 用户列表加载更多用户
};


#endif //CLIENT_CHATDIALOG_H
