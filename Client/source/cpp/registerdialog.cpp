#include "global.h"
#include "httpmanager.h"
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

    // 连接信号槽
    connect(HttpManager::GetInstance().get(), &HttpManager::signal_register_finish,
            this, &RegisterDialog::slot_register_finish);

    initHttpHandlers();

    // 鼠标移到[眼睛]位置，将会变成[小手]
    ui->password_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);
    // 设置多种状态样式
    ui->password_visible->SetState("unvisible", "unvisible_hover", "",
                                   "visible", "visible_hover", "");

    ui->confirm_visible->SetState("unvisible", "unvisible_hover", "",
                                  "visible", "visible_hover", "");
    // 连接点击事件
    connect(ui->password_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->password_visible->GetCurState();
        if (ClickLbState::Normal == state) {
            ui->password_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->password_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "passwd label was clicked!";
    });
    connect(ui->confirm_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->confirm_visible->GetCurState();
        if (state == ClickLbState::Normal) {
            ui->confirm_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->confirm_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "confirm abel was clicked!";
    });
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
        QJsonObject json;

        json["email"] = email;
        HttpManager::GetInstance()->PostHttpRequest(QUrl(url_prefix + "/get_varify_code"),
                                                    json, RequestId::ID_GET_VARIFY_CODE,
                                                    Module::REGISTERMOD);
    } else {
        showTip(tr("邮箱地址错误"), false);
    }
}

void RegisterDialog::slot_register_finish(RequestId id, QString res, ErrorCodes err) {
    if (ErrorCodes::SUCCESS != err) {
        showTip(tr("网络请求错误"), false);
        return;
    }

    // 解析 json 字符串，将
    QJsonDocument json = QJsonDocument::fromJson(res.toUtf8());
    if (json.isNull()) {
        showTip(tr("json解析失败"), false);
        return;
    }
    if (!json.isObject()) {
        showTip(tr("json解析失败"), false);
        return;
    }
    _handlers[id](json.object());
    return;
}

void RegisterDialog::on_sure_btn_clicked() {
    if (ui->user_edit->text() == "") {
        showTip(tr("用户名不能为空"), false);
        return;
    }

    if (ui->email_edit->text() == "") {
        showTip(tr("邮箱不能为空"), false);
        return;
    }

    if (ui->password_edit->text() == "") {
        showTip(tr("密码不能为空"), false);
        return;
    }

    if (ui->confirm_edit->text() == "") {
        showTip(tr("确认密码不能为空"), false);
        return;
    }

    if (ui->confirm_edit->text() != ui->password_edit->text()) {
        showTip(tr("密码和确认密码不匹配"), false);
        return;
    }

    if (ui->varify_edit->text() == "") {
        showTip(tr("验证码不能为空"), false);
        return;
    }

    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = md5Encrypt(ui->password_edit->text());
    json_obj["confirm"] = md5Encrypt(ui->confirm_edit->text());
    json_obj["varifycode"] = ui->varify_edit->text();
    HttpManager::GetInstance()->PostHttpRequest(QUrl(url_prefix + "/user_register"),
                                                json_obj, RequestId::ID_REGISTER_USER, Module::REGISTERMOD);
}

void RegisterDialog::initHttpHandlers() {
    // 获取验证码回包
    _handlers.insert(RequestId::ID_GET_VARIFY_CODE, [this](const QJsonObject &json) {
        int error = json["error"].toInt();
        if (ErrorCodes::SUCCESS != error) {
            showTip(tr("参数错误"), false);
            return;
        }

        auto email = json["email"].toString();
        showTip(tr("验证码已发送到邮箱，注意查收"), true);
        qDebug() << "email is: " << email;
    });

    // 注册用户回包逻辑
    _handlers.insert(RequestId::ID_REGISTER_USER, [this](QJsonObject json) {
        int error = json["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误"), false);
            return;
        }
        showTip(tr("用户注册成功"), true);
        qDebug() << "user uid is: " << json["uid"].toInt();
        qDebug() << "email is: " << json["email"].toString();
    });
}
