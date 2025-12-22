#include "logindialog.h"
#include "../forms/ui_LoginDialog.h"


LoginDialog::LoginDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);

    // 按下登录界面的 [注册按钮] 发送 switchRegister 信号
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
}

LoginDialog::~LoginDialog() {
    delete ui;
}
