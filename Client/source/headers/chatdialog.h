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
#include "statewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChatDialog; }
QT_END_NAMESPACE

class ChatDialog : public QDialog {
Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);                         // 构造函数
    ~ChatDialog() override;                                                 // 析构函数
    void AddChatUserList();                                                 // 添加聊天界面用户列表
    void ClearLabelState(StateWidget *widget);                              // 清空标签状态
private:
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    QList<StateWidget *> _lb_list;

private:
    void showSearch(bool b_search = false);                                 // 显示各种列表
    void addLBGroup(StateWidget *widget);                                   // 加入按钮组

private slots:

    void slot_loading_chat_user();                                          // 用户列表加载更多用户
    void slot_side_chat();                                                  // 侧边栏聊天图标
    void slot_side_contact();                                               // 侧边栏联系图标
    void slot_text_changed(const QString &str);                             //
};


#endif //CLIENT_CHATDIALOG_H
