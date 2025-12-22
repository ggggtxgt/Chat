/******************************************************************************
 * @file       logindialog.h
 * @brief      登录界面头文件
 * @author     ggggtxgt
 * @date       2025/12/22 13:27
 * @history
*****************************************************************************/
#ifndef CLIENT_LOGINDIALOG_H
#define CLIENT_LOGINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog {
Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

    ~LoginDialog() override;

private:
    Ui::LoginDialog *ui;

signals:

    void switchRegister();          // 切换到注册界面
};


#endif //CLIENT_LOGINDIALOG_H
