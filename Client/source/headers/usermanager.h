/******************************************************************************
 * @file       usermanager.h
 * @brief      用户信息管理类头文件
 * @author     ggggtxgt
 * @date       2025/12/29 18:21
 * @history
*****************************************************************************/
#ifndef CLIENT_USERMANAGER_H
#define CLIENT_USERMANAGER_H

#include <memory>

#include <QObject>

#include "singleton.h"

class UserManager : public QObject, public Singleton<UserManager>,
                    public std::enable_shared_from_this<UserManager> {
Q_OBJECT

public:
    friend class Singleton<UserManager>;

    ~UserManager() override;                                // 析构函数
    void SetUid(int uid);                                   // 设置 uid
    void SetName(QString name);                             // 设置名称
    void SetToken(QString token);                           // 设置 token

private:
    UserManager();

    int _uid;
    QString _name;
    QString _token;
};


#endif //CLIENT_USERMANAGER_H
