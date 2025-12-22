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

private:
    void showTip(QString str, bool b_ok);           // 展示[错误提示]标签
    void initHttpHandlers();                        // 初始化 HTTP 请求处理器

private slots:

    void on_get_code_clicked();                     // [获取验证码] 按钮对应的槽函数

    /******************************************************************************
     * @func      slot_register_finish
     * @brief     用来接收并处理注册相关 HTTP 请求结果
     * @param     HTTP 请求的不同目的
     * @param     服务器返回的结果（一般为 JSON 格式）
     * @param     HTTP 请求发送失败的错误类型
    *****************************************************************************/
    void slot_register_finish(RequestId id, QString res, ErrorCodes err);
};


#endif //CLIENT_REGISTERDIALOG_H
