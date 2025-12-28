#include "mainwindow.h"

// 注意 ui_xxx.h 文件的包含：如果所有文件都位于一个目录，则可以使用 #include "ui_xxx.h” 的形式
// 如果文件位于不同的文件夹，则必须指定其位置：#include "../forms/ui_xxx.h" 的形式，否则将会出错
#include "../forms/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 创建登录、注册界面实例
    _login_dialog = new LoginDialog(this);
    _login_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dialog);

    _register_dialog = new RegisterDialog(this);

    // 建立 [界面切换: 登录 --> 注册] 的信号连接
    connect(_login_dialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SlotSwitchRegister() {
    _register_dialog = new RegisterDialog(this);
    _register_dialog->hide();
    _register_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // 连接注册界面返回信号
    connect(_register_dialog, &RegisterDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);
    // 设置主界面的内部界面为注册界面；隐藏登录界面；显示注册界面
    setCentralWidget(_register_dialog);
    _login_dialog->hide();
    _register_dialog->show();
}

void MainWindow::SlotSwitchLogin() {
    // 创建 CentralWidget，并将其设置为 MainWindow 的中心部件
    _login_dialog = new LoginDialog(this);
    _login_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    setCentralWidget(_login_dialog);

    _register_dialog->hide();
    _login_dialog->show();
    // 连接登录界面注册信号
    connect(_login_dialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);
}
