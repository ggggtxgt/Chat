/******************************************************************************
 * @file       resetdialog.h
 * @brief      重置密码界面头文件
 * @author     ggggtxgt
 * @date       2025/12/28 21:40
 * @history
*****************************************************************************/
#ifndef CLIENT_RESETDIALOG_H
#define CLIENT_RESETDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QRegularExpression>

#include "global.h"
#include "httpmanager.h"
#include "registerdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ResetDialog; }
QT_END_NAMESPACE

class ResetDialog : public QDialog {
Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);

    ~ResetDialog() override;

private:
    void initHandlers();                                // 初始化处理器
    bool checkUserValid();                              // 检查用户信息
    bool checkPassValid();                              // 检查密码信息
    bool checkEmailValid();                             // 检查邮箱信息
    bool checkVarifyValid();                            // 检查验证码信息
    void delTipErr(TipErr te);                          // 删除错误提示信息
    void showTip(QString str, bool b_ok);               // 展示错误提示信息
    void addTipErr(TipErr te, QString tips);            // 添加错误提示信息

private:
    Ui::ResetDialog *ui;
    QMap<TipErr, QString> _tip_errs;
    QMap<RequestId, std::function<void(const QJsonObject &)>> _handlers;

private slots:

    void on_sure_btn_clicked();                                 // [确认]按钮
    /*
    void on_cancel_btn_clicked();                               // [取消]按钮
    **/
    void on_varify_btn_clicked();                               // [获取]按钮
    void on_return_btn_clicked();                               // [返回]按钮
    void slot_reset_finish(RequestId, QString, ErrorCodes);     // [忘记密码]按钮

signals:

    void switchLogin();
};


#endif //CLIENT_RESETDIALOG_H
