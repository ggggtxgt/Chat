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

#include "global.h"

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

    int _uid;
    QString _token;
    QMap<TipErr, QString> _tip_errs;
    QMap<RequestId, std::function<void(const QJsonObject &)>> _handlers;

private:
    void initHead();                                // 初始化头部
    void initHttpHandlers();                        // 初始化 HTTP 处理器
    bool checkPwdValid();                           // 检查密码
    bool checkEmailValid();                         // 检查邮箱
    bool enableBtn(bool enabled);                   // 按钮是否可用
    void delTipErr(TipErr err);                     // 删除错误提示信息
    void showTip(QString str, bool b_ok);           // 展示错误提示信息
    void addTipErr(TipErr err, QString tips);       // 添加错误提示信息

public slots:

    void slot_forget_pwd();                                 // [忘记密码]对应的槽函数
    void on_login_btn_clicked();                            // [登录]按钮对应的槽函数
    void slot_login_finish(RequestId, QString, ErrorCodes); //

signals:

    void switchRegister();                                  // 切换到注册界面
    void switchReset();                                     // 切换到重置界面
    void signal_connect_tcp(ServerInfo si);                 //
    void signal_login_finish();                             //
};


#endif //CLIENT_LOGINDIALOG_H
