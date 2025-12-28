/******************************************************************************
 * @file       timerbtn.h
 * @brief      定时按钮头文件
 * @author     ggggtxgt
 * @date       2025/12/28 14:30
 * @history
*****************************************************************************/
#ifndef CLIENT_TIMERBTN_H
#define CLIENT_TIMERBTN_H

#include <QTimer>
#include <QPushButton>

#include "global.h"

class TimerBtn : public QPushButton {
Q_OBJECT
public:
    TimerBtn(QWidget *parent);                                      // 构造函数
    ~TimerBtn();                                                    // 析构函数
    virtual void mouseReleaseEvent(QMouseEvent *event) override;    // 重写鼠标释放事件

private:
    QTimer *_timer;
    int _counter;
};


#endif //CLIENT_TIMERBTN_H
