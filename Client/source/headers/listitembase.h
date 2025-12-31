/******************************************************************************
 * @file       listitembase.h
 * @brief      所有自定义 item 的基类
 * @author     ggggtxgt
 * @date       2025/12/30 18:48
 * @history
*****************************************************************************/
#ifndef CLIENT_LISTITEMBASE_H
#define CLIENT_LISTITEMBASE_H

#include "global.h"

#include <QWidget>

class ListItemBase : public QWidget {
Q_OBJECT
public:
    explicit ListItemBase(QWidget *parent = nullptr);                   // 构造函数
    ~ListItemBase();                                                    // 析构函数
    void SetItemType(ListItemType type);                                // 设置 item 类型
    ListItemType GetItemType();                                         // 获取 item 类型

protected:
    virtual void paintEvent(QPaintEvent *event) override;               // 重写绘画事件

private:
    ListItemType _type;
};


#endif //CLIENT_LISTITEMBASE_H
