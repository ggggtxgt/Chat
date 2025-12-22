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
    LoginDialog *_login_dialog;
    RegisterDialog *_register_dialog;

private slots:

    void SlotSwitchRegister();
};

#endif // MAINWINDOW_H