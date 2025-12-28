#include "httpmanager.h"

HttpManager::~HttpManager() {}

HttpManager::HttpManager() {
    connect(this, &HttpManager::signal_http_finish, this, &HttpManager::slot_http_finish);
}

void HttpManager::PostHttpRequest(QUrl url, QJsonObject json, RequestId id, Module module) {
    // 将 QJsonObject 转换为 json 数据
    QByteArray data = QJsonDocument(json).toJson();
    // 构建 url，设置头部信息（数据类型，数据长度）
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));

    // 使用 shared_from_this 确保单例对象不被提前析构
    auto self = shared_from_this();
    QNetworkReply *reply = _manager.post(request, data);
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, id, module] {
        // 处理错误情况
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "PostHttpRequest Error: " << reply->errorString();

            // 即使错误，也应通知其他模块发送完毕
            emit self->signal_http_finish(id, "", ErrorCodes::ERR_NETWORK, module);
            reply->deleteLater();       // 释放 reply
            return;
        }

        // 没有错误，正常处理
        QString res = reply->readAll();
        // 发送成功，通知其他模块发送完毕
        emit self->signal_http_finish(id, res, ErrorCodes::SUCCESS, module);
        reply->deleteLater();
        return;
    });
}

void HttpManager::slot_http_finish(RequestId id, QString res, ErrorCodes err, Module module) {
    if (Module::REGISTERMOD == module) {
        // 发送信号通知指定模块，http 响应已经结束
        emit signal_register_finish(id, res, err);
    }
    if (Module::RESETMOD == module) {
        // 发送信号通知指定模块，http 响应结束
        emit signal_reset_finish(id, res, err);
    }
}
