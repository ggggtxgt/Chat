#include "mainwindow.h"
#include "logindialog.h"

// 注意 ui_xxx.h 文件的包含：如果所有文件都位于一个目录，则可以使用 #include "ui_xxx.h” 的形式
// 如果文件位于不同的文件夹，则必须指定其位置：#include "../forms/ui_xxx.h" 的形式，否则将会出错
#include "../forms/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    LoginDialog *login_dialog = new LoginDialog(this);
    setCentralWidget(login_dialog);
    login_dialog->show();
}

MainWindow::~MainWindow() {
    delete ui;
}