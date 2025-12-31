/******************************************************************************
 * @file       chatview.h
 * @brief      聊天视图头文件
 * @author     ggggtxgt
 * @date       2025/12/31 12:28
 * @history
*****************************************************************************/
#ifndef CLIENT_CHATVIEW_H
#define CLIENT_CHATVIEW_H

#include <QTimer>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>

class ChatView : public QWidget {
Q_OBJECT

public:
    explicit ChatView(QWidget *parent = nullptr);                               // 构造函数
    ~ChatView() override;                                                       // 析构函数
    void AppendChatItem(QWidget *item);                                         // 从尾部插入
    void PrependChatItem(QWidget *item);                                        // 从头部插入
    void InsertChatItem(QWidget *item);                                         // 从中间插入

protected:
    bool eventFilter(QObject *object, QEvent *event) override;                  // 重写事件过滤器
    void paintEvent(QPaintEvent *event) override;                               // 重写绘制事件

private:
    void initStyleSheet();                                                      // 初始化样式

private slots:

    void onVScrollBarMoved(int min, int max);

private:
    bool _is_append;
    QScrollArea *_area;
    QVBoxLayout *_layout;
};


#endif //CLIENT_CHATVIEW_H
