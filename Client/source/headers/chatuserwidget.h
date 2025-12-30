/******************************************************************************
 * @file       chatuserwidget.h
 * @brief      聊天界面用户列表头文件
 * @author     ggggtxgt
 * @date       2025/12/30 18:42
 * @history
*****************************************************************************/
#ifndef CLIENT_CHATUSERWIDGET_H
#define CLIENT_CHATUSERWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class ChatUserWidget; }
QT_END_NAMESPACE

class ChatUserWidget : public QWidget {
Q_OBJECT

public:
    explicit ChatUserWidget(QWidget *parent = nullptr);

    ~ChatUserWidget() override;

private:

};


#endif //CLIENT_CHATUSERWIDGET_H
