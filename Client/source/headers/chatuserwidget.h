/******************************************************************************
 * @file       chatuserwidget.h
 * @brief      聊天界面用户列表头文件
 * @author     ggggtxgt
 * @date       2025/12/30 18:42
 * @history
*****************************************************************************/
#ifndef CLIENT_CHATUSERWIDGET_H
#define CLIENT_CHATUSERWIDGET_H

#include "ListItemBase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChatUserWidget; }
QT_END_NAMESPACE

class ChatUserWidget : public ListItemBase {
Q_OBJECT

public:
    explicit ChatUserWidget(QWidget *parent = nullptr);                     // 构造函数
    ~ChatUserWidget() override;                                             // 析构函数
    QSize sizeHint() const override;                                        // 返回自定义尺寸
    void SetInfo(QString name, QString head, QString msg);                  // 设置 item 信息

private:
    Ui::ChatUserWidget *ui;
    QString _name;
    QString _head;
    QString _msg;
};


#endif //CLIENT_CHATUSERWIDGET_H
