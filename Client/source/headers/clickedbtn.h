/******************************************************************************
 * @file       clickedbtn.h
 * @brief      自定义可点击按钮头文件
 * @author     ggggtxgt
 * @date       2025/12/30 15:17
 * @history
*****************************************************************************/
#ifndef CLIENT_CLICKEDBTN_H
#define CLIENT_CLICKEDBTN_H

#include <QPushButton>


class ClickedBtn : public QPushButton {
Q_OBJECT

public:
    explicit ClickedBtn(QWidget *parent = nullptr);                         // 构造函数
    ~ClickedBtn() override;                                                 // 析构函数
    void SetState(QString normal, QString hover, QString press);            // 设置按钮状态

protected:
    virtual void enterEvent(QEvent *event) override;                        // 鼠标进入
    virtual void leaveEvent(QEvent *event) override;                        // 鼠标离开
    virtual void mousePressEvent(QMouseEvent *event) override;              // 鼠标按下
    virtual void mouseReleaseEvent(QMouseEvent *event) override;            // 鼠标释放

private:
    QString _hover;
    QString _press;
    QString _normal;
};


#endif //CLIENT_CLICKEDBTN_H
