/******************************************************************************
 * @file       statewidget.h
 * @brief      XXXX Function
 * @author     ggggtxgt
 * @date       2026/1/1 13:08
 * @history    
*****************************************************************************/
#ifndef CLIENT_STATEWIDGET_H
#define CLIENT_STATEWIDGET_H

#include <QLabel>
#include <QWidget>

#include "global.h"

class StateWidget : public QWidget {
Q_OBJECT

public:
    explicit StateWidget(QWidget *parent = nullptr);                            // 构造函数
    ~StateWidget();                                                             // 析构函数
    void ClearState();                                                          // 清空控件状态
    void AddRedPoint();                                                         // 添加中红点到控件
    void SetSelected(bool selected);                                            // 设置控件状态
    void ShowRedPoint(bool show = true);                                        // 显示或隐藏红点
    ClickLbState GetCurState();                                                 // 返回当前状态
    void SetState(QString normal = "", QString hover = "", QString press = "",
                  QString select = "", QString select_hover = "", QString select_press = "");

protected:
    void paintEvent(QPaintEvent *event);                                        // 重写绘制事件
    virtual void mousePressEvent(QMouseEvent *event) override;                  // 重写鼠标按下事件
    virtual void mouseReleaseEvent(QMouseEvent *event) override;                // 重写鼠标释放事件
    virtual void enterEvent(QEvent *event) override;                            // 重写鼠标进入事件
    virtual void leaveEvent(QEvent *event) override;                            // 重写鼠标离开事件

private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;
    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLbState _cur_state;
    QLabel *_red_point;

signals:

    void clicked(void);
};


#endif //CLIENT_STATEWIDGET_H
