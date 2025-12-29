/******************************************************************************
 * @file       httpmanager.h
 * @brief      HTTP管理者头文件
 * @author     ggggtxgt
 * @date       2025/12/22 18:57
 * @history
*****************************************************************************/
#ifndef CLIENT_HTTPMANAGER_H
#define CLIENT_HTTPMANAGER_H

#include <QUrl>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "singleton.h"

class HttpManager : public QObject, public Singleton<HttpManager>,
                    public std::enable_shared_from_this<HttpManager> {
Q_OBJECT

public:
    ~HttpManager();                         // 析构函数

    /******************************************************************************
 * @func      PostHttpRequest
 * @brief     发送 HTTP 请求
 * @param     需要发送的路由地址
 * @param     需要发送的 JSON 数据
 * @param     HTTP 请求的不同目的
 * @param     发送 HTTP 请求的模块
*****************************************************************************/
    void PostHttpRequest(QUrl url, QJsonObject json, RequestId id, Module module);

private:
    friend class Singleton<HttpManager>;    // 指定友元
    HttpManager();                          // 构造函数

private:
    QNetworkAccessManager _manager;

private slots:

    void slot_http_finish(RequestId id, QString res, ErrorCodes err, Module module);

signals:

    /******************************************************************************
     * @func      createThread
     * @brief     一个 HTTP 请求发送完毕
     * @param     HTTP 请求的不同目的
     * @param     发送的结果：成功--失败
     * @param     HTTP 请求发送失败的错误类型
     * @param     发送 HTTP 请求的模块
    *****************************************************************************/
    void signal_http_finish(RequestId id, QString res, ErrorCodes err, Module module);

    void signal_register_finish(RequestId id, QString res, ErrorCodes err);

    void signal_reset_finish(RequestId id, QString str, ErrorCodes code);

    void signal_login_finish(RequestId id, QString res, ErrorCodes code);
};


#endif //CLIENT_HTTPMANAGER_H
