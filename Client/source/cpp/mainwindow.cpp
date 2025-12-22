#include "mainwindow.h"

// 注意 ui_xxx.h 文件的包含：如果所有文件都位于一个目录，则可以使用 #include "ui_xxx.h” 的形式
// 如果文件位于不同的文件夹，则必须指定其位置：#include "../forms/ui_xxx.h" 的形式，否则将会出错
#include "../forms/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 创建登录、注册界面实例
    _login_dialog = new LoginDialog();
    setCentralWidget(_login_dialog);
    _login_dialog->show();

    _register_dialog = new RegisterDialog();

    // 建立 [界面切换: 登录 --> 注册] 的信号连接
    connect(_login_dialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);
}

MainWindow::~MainWindow() {
    delete ui;

    // 释放登录、注册界面申请的资源，并且防止出现野指针
    if (_login_dialog) {
        delete _login_dialog;
        _login_dialog = nullptr;
    }
    if (_register_dialog) {
        delete _register_dialog;
        _register_dialog = nullptr;
    }
}

void MainWindow::SlotSwitchRegister() {
    // 设置主界面的内部界面为注册界面；隐藏登录界面；显示注册界面
    setCentralWidget(_register_dialog);
    _login_dialog->hide();
    _register_dialog->show();
}
