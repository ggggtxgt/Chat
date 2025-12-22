/******************************************************************************
 * @file       global.h
 * @brief      全局使用的函数、变量……
 * @author     ggggtxgt
 * @date       2025/12/22 16:38
 * @history
*****************************************************************************/
#ifndef CLIENT_GLOBAL_H
#define CLIENT_GLOBAL_H

#include <QStyle>
#include <QWidget>
#include <QRegularExpression>

#include <functional>

// 刷新qss，从而更新错误提示
extern std::function<void(QWidget *)> repolish;

#endif //CLIENT_GLOBAL_H
