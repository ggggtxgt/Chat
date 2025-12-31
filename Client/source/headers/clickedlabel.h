/******************************************************************************
 * @file       clickedlabel.h
 * @brief      可点击标签头文件
 * @author     ggggtxgt
 * @date       2025/12/28 15:40
 * @history
*****************************************************************************/
#ifndef CLIENT_CLICKEDLABEL_H
#define CLIENT_CLICKEDLABEL_H

#include <QLabel>

#include "global.h"

class ClickedLabel : public QLabel {
Q_OBJECT
public:
    ClickedLabel(QWidget *parent);                                      // 构造函数
    ClickLbState GetCurState();                                         // 获取当前状态
    virtual void leaveEvent(QEvent *event) override;                    // 鼠标离开事件
    virtual void mouseReleaseEvent(QMouseEvent *event);                 // 鼠标释放事件
    virtual void enterEvent(QEvent *event) override;                    // 鼠标进入事件
    virtual void mousePressEvent(QMouseEvent *event) override;          // 鼠标点击事件
    void SetState(QString normal = "", QString hover = "",
                  QString press = "", QString select = "",
                  QString select_hover = "", QString select_press = "");// 设置当前状态


private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLbState _curstate;

signals:

    void clicked(void);
};

#endif //CLIENT_CLICKEDLABEL_H
