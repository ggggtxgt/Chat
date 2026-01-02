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
    void ClearState();                                                          //
    void AddRedPoint();                                                         //
    void SetSelected(bool selected);                                                         //
    void ShowRedPoint(bool show = true);                                        //
    ClickLbState GetCurState();                                                 //
    void SetState(QString normal = "", QString hover = "", QString press = "",
                  QString select = "", QString select_hover = "", QString select_press = "");

protected:
    void paintEvent(QPaintEvent *event);                                        //
    virtual void mousePressEvent(QMouseEvent *event) override;                  //
    virtual void mouseReleaseEvent(QMouseEvent *event) override;                //
    virtual void enterEvent(QMouseEvent *event) override;                       //
    virtual void leaveEvent(QMouseEvent *event) override;                       //

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
