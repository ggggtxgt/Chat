/******************************************************************************
 * @file       global.h
 * @brief      全局使用的函数、变量……
 * @author     ggggtxgt
 * @date       2025/12/22 16:38
 * @history
*****************************************************************************/
#ifndef CLIENT_GLOBAL_H
#define CLIENT_GLOBAL_H

#include <QDebug>
#include <QStyle>
#include <QWidget>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QRegularExpression>

#include <mutex>
#include <memory>
#include <iostream>
#include <functional>

// 刷新qss，从而更新错误提示
extern std::function<void(QWidget *)> repolish;

// HTTP 请求的不同目的
enum RequestId {
    ID_GET_VARIFY_CODE = 1001,              // 获取验证码
    ID_REGISTER_USER = 1002,                // 注册用户
};

// 发送 HTTP 请求的模块
enum Module {
    REGISTERMOD = 0,                        // 注册
};

// HTTP 请求发送失败的错误类型
enum ErrorCodes {
    SUCCESS = 0,                            // 成功
    ERR_JSON = 1,                           // JSON 解析失败
    ERR_NETWORK = 2,                        // 网络错误
};

// label 的两种基本状态
enum ClickLbState{
    Normal = 0,
    Selected = 1
};

// 使用 md5 对密码进行加密
extern QString md5Encrypt(const QString &input);

// 发送 post 请求的网络前缀
extern QString url_prefix;

#endif //CLIENT_GLOBAL_H
