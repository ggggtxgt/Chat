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
    ID_GET_VARIFY_CODE = 1001,                  // 获取验证码
    ID_REGISTER_USER = 1002,                    // 注册用户
    ID_RESET_PWD = 1003,                        // 重置密码
    ID_LOGIN_USER = 1004,                       // 用户登录
    ID_CHAT_LOGIN = 1005,                       // 登录聊天服务器
    ID_CHAT_LOGIN_RSP = 1006,                   // 登录聊天服务器回包
    ID_SEARCH_USER_REQ = 1007,                  // 用户搜索请求
    ID_SEARCH_USER_RSP = 1008,                  // 搜索用户回包
    ID_ADD_FRIEND_REQ = 1009,                   // 添加好友申请
    ID_ADD_FRIEND_RSP = 1010,                   // 申请添加好友回复
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,            // 通知用户添加好友申请
    ID_AUTH_FRIEND_REQ = 1013,                  // 认证好友请求
    ID_AUTH_FRIEND_RSP = 1014,                  // 认证好友回复
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015,           // 通知用户认证好友申请
    ID_TEXT_CHAT_MSG_REQ  = 1017,               // 文本聊天信息请求
    ID_TEXT_CHAT_MSG_RSP  = 1018,               // 文本聊天信息回复
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019,         // 通知用户文本聊天信息
    ID_NOTIFY_OFF_LINE_REQ = 1021,              // 通知用户下线
    ID_HEART_BEAT_REQ = 1023,                   // 心跳请求
    ID_HEARTBEAT_RSP = 1024,                    // 心跳回复
};

// 发送 HTTP 请求的模块
enum Module {
    REGISTERMOD = 0,            // 注册
    RESETMOD = 1,               // 重置密码
    LOGINMOD = 2,               // 登录
};

// HTTP 请求发送失败的错误类型
enum ErrorCodes {
    SUCCESS = 0,                            // 成功
    ERR_JSON = 1,                           // JSON 解析失败
    ERR_NETWORK = 2,                        // 网络错误
};

// 错误提示的枚举类型
enum TipErr {
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};

// label 的两种基本状态
enum ClickLbState {
    Normal = 0,
    Selected = 1
};

// 服务器返回的信息
struct ServerInfo {
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

// 聊天界面的模式
enum ChatUIMode {
    SearchMode,                 // 搜索模式
    ChatMode,                   // 聊天模式
    ContactMode,                // 联系模式
};

// 自定义 QListWidgetItem 的几种类型
enum ListItemType {
    CHAT_USER_ITEM,             // 聊天用户
    CONTACT_USER_ITEM,          // 联系人用户
    SEARCH_USER_ITEM,           // 搜索到的用户
    ADD_USER_TIP_ITEM,          // 提示添加用户
    INVALID_ITEM,               // 不可点击条目
    GROUP_TIP_ITEM,             // 分组提示条目
    LINE_ITEM,                  // 分割线
    APPLY_FRIEND_ITEM,          // 好友申请
};

enum class ChatRole {
    Self,                       // 自己
    Other,                      // 他人
};

struct MsgInfo {
    QString msg_flag;           // text image file
    QPixmap pixmap;             // 文件和图片的缩略图
    QString content;            // 表示文件和图像的url，文本信息
};

// 使用 md5 对密码进行加密
extern QString md5Encrypt(const QString &input);

// 发送 post 请求的网络前缀
extern QString url_prefix;

// 申请好友标签输入框最短长度
const int MIN_APPLY_LABEL_ED_LEN = 40;
const QString add_prefix = "添加标签";
const int tip_offset = 5;

const int CHAT_COUNT_PER_PAGE = 13;

// *************************************************************************************************
extern std::vector<QString> strs;
extern std::vector<QString> heads;
extern std::vector<QString> names;
// *************************************************************************************************

#endif //CLIENT_GLOBAL_H
