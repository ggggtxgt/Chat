/******************************************************************************
 * @file       textbubble.h
 * @brief      文本气泡框头文件
 * @author     ggggtxgt
 * @date       2025/12/31 16:23
 * @history
*****************************************************************************/
#ifndef CLIENT_TEXTBUBBLE_H
#define CLIENT_TEXTBUBBLE_H

#include <QTextEdit>
#include <QHBoxLayout>

#include "bubbleframe.h"

class TextBubble : public BubbleFrame {
Q_OBJECT

public:
    TextBubble(ChatRole role, const QString &text, QWidget *parent = nullptr);  // 构造函数
    ~TextBubble();                                                              // 析构函数

protected:
    bool eventFilter(QObject *object, QEvent *event);                           // 重写事件过滤器

private:
    void adjustTextHeight();                                                    //
    void setPlainText(const QString &text);                                     //
    void initStyleSheet();                                                      //

private:
    QTextEdit *_text_edit;
};


#endif //CLIENT_TEXTBUBBLE_H
