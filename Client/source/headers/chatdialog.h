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


QT_BEGIN_NAMESPACE
namespace Ui { class ChatDialog; }
QT_END_NAMESPACE

class ChatDialog : public QDialog {
Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);

    ~ChatDialog() override;

private:
    Ui::ChatDialog *ui;
};


#endif //CLIENT_CHATDIALOG_H
