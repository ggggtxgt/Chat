#include "logindialog.h"
#include "clickedlabel.h"
#include "../forms/ui_LoginDialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);

    // 按下登录界面的 [注册按钮] 发送 switchRegister 信号
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
    // 设置[忘记密码]对应的状态属性
    ui->forget_label->SetState("normal", "hover", "", "selected", "selected_hover", "");
    connect(ui->forget_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);
}

LoginDialog::~LoginDialog() {
    delete ui;
}

void LoginDialog::slot_forget_pwd() {
    qDebug() << "slot forget pwd";
    emit switchReset();
}
