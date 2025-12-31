/******************************************************************************
 * @file       picturebubble.h
 * @brief      图片气泡框头文件
 * @author     ggggtxgt
 * @date       2025/12/31 17:55
 * @history
*****************************************************************************/
#ifndef CLIENT_PICTUREBUBBLE_H
#define CLIENT_PICTUREBUBBLE_H

#include "bubbleframe.h"

#include <QPixmap>
#include <QHBoxLayout>

class PictureBubble : public BubbleFrame {
Q_OBJECT

public:
    PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent = nullptr);    // 构造函数
    ~PictureBubble();                                                                   // 析构函数
};


#endif //CLIENT_PICTUREBUBBLE_H
