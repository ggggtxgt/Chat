#include "global.h"
#include "registerdialog.h"
#include "../forms/ui_RegisterDialog.h"


RegisterDialog::RegisterDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::RegisterDialog) {
    ui->setupUi(this);

    // 设置注册界面密码输入模式为[密码模式]---密码显示为黑圆
    ui->password_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);

    // 设置[错误提示]标签的状态属性
    ui->error_tip->setProperty("state", "normal");
}

RegisterDialog::~RegisterDialog() {
    delete ui;
}

void RegisterDialog::showTip(QString str, bool b_ok) {
    if (b_ok) {
        ui->error_tip->setProperty("state", "normal");
    } else {
        ui->error_tip->setProperty("state", "error");
    }
    ui->error_tip->setText(str);
    repolish(ui->error_tip);
}

void RegisterDialog::on_get_code_clicked() {
    auto email = ui->email_edit->text();
    // 定义正则表达式规则
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if (match) {
        // 发送Http验证码
        // @todo 之后完善
    } else {
        showTip(tr("邮箱地址错误"), false);
    }
}
