/******************************************************************************
 * @file       registerdialog.h
 * @brief      注册界面头文件
 * @author     ggggtxgt
 * @date       2025/12/22 13:48
 * @history
*****************************************************************************/
#ifndef CLIENT_REGISTERDIALOG_H
#define CLIENT_REGISTERDIALOG_H

#include <QMap>
#include <QDialog>

#include "global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RegisterDialog; }
QT_END_NAMESPACE

class RegisterDialog : public QDialog {
Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

    ~RegisterDialog() override;

private:
    Ui::RegisterDialog *ui;

    // key--请求目的；value--函数对象
    QMap<RequestId, std::function<void(const QJsonObject &)>> _handlers;
    QMap<TipErr, QString> _tip_errs;                        // 错误提示映射表
    QTimer *_countdown_timer;                               // 倒计时计时器
    int _countdown;                                         // 回到登录界面的倒计时

private:
    void showTip(QString str, bool b_ok);           // 展示[错误提示]标签
    void initHttpHandlers();                        // 初始化 HTTP 请求处理器
    void addTipErr(TipErr te, QString tips);        // 添加错误信息
    void delTipErr(TipErr te);                      // 删除错误信息
    bool checkUserValid();                          // 检查用户信息
    bool checkEmailValid();                         // 检查邮箱信息
    bool checkPassValid();                          // 检查密码信息
    bool checkConfirmValid();                       // 检查重复密码信息
    bool checkVarifyValid();                        // 检查验证码信息
    void changeTipPage();                           // 注册成功，回到登录界面

signals:

    void sigSwitchLogin();

private slots:

    void on_cancel_btn_clicked();                                   // 注册界面[取消]按钮
    void on_return_btn_clicked();                                   // 注册界面[返回]按钮
    void on_sure_btn_clicked();                                     // 注册登录[确认]按钮
    void on_get_code_clicked();                                     // [获取验证码] 按钮对应的槽函数
    void slot_register_finish(RequestId, QString, ErrorCodes);      // 接收并处理注册相关 HTTP 请求结果

};


#endif //CLIENT_REGISTERDIALOG_H
