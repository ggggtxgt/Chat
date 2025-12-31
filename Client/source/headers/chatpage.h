/******************************************************************************
 * @file       chatpage.h
 * @brief      自定义聊天页头文件
 * @author     ggggtxgt
 * @date       2025/12/31 10:12
 * @history
*****************************************************************************/
#ifndef CLIENT_CHATPAGE_H
#define CLIENT_CHATPAGE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class ChatPage; }
QT_END_NAMESPACE

class ChatPage : public QWidget {
Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);

    ~ChatPage() override;

private:
    Ui::ChatPage *ui;
};


#endif //CLIENT_CHATPAGE_H
