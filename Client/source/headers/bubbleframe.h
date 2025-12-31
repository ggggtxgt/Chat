/******************************************************************************
 * @file       bubbleframe.h
 * @brief      气泡窗口的头文件
 * @author     ggggtxgt
 * @date       2025/12/31 14:48
 * @history
*****************************************************************************/
#ifndef CLIENT_BUBBLEFRAME_H
#define CLIENT_BUBBLEFRAME_H

#include <QFrame>
#include <QHBoxLayout>

#include "global.h"

class BubbleFrame : public QFrame {
Q_OBJECT
public:
    BubbleFrame(ChatRole role, QWidget *parent = nullptr);                      // 构造函数
    ~BubbleFrame();                                                             // 析构函数
    void SetMargin(int margin);                                                 // 设置边距
    void SetWidget(QWidget *widget);                                            // 设置窗口

protected:
    void paintEvent(QPaintEvent *event);                                        // 重写绘制事件

private:
    int _margin;
    ChatRole _role;
    QHBoxLayout *_hb_layout;
};


#endif //CLIENT_BUBBLEFRAME_H
