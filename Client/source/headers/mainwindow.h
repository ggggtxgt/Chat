/******************************************************************************
 * @file       mainwindow.h
 * @brief      主窗口头文件
 * @author     ggggtxgt
 * @date       2025/12/22 12:38
 * @history
*****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "chatdialog.h"
#include "resetdialog.h"
#include "logindialog.h"
#include "registerdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ChatDialog *_chat_dialog;           // 聊天界面类对象
    ResetDialog *_reset_dialog;         // 重置密码类对象
    LoginDialog *_login_dialog;         // 登录界面类对象
    RegisterDialog *_register_dialog;   // 注册界面类对象

private slots:

    void SlotSwitchLogin();             // 槽函数：[注册界面] ---> [登录界面]
    void SlotSwitchRegister();          // 槽函数：[登录界面] ---> [注册界面]
    void SlotSwitchReset();             // 槽函数：[登录界面] ---> [重置界面]
    void SlotSwitchLogin2();            // 槽函数：[重置界面] ---> [登录界面]
    void SlotSwitchChat();              // 槽函数：[登录界面] ---> [聊天界面]
};

#endif // MAINWINDOW_H