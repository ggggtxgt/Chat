/******************************************************************************
 * @file       customizeedit.h
 * @brief      自定义编辑框头文件
 * @author     ggggtxgt
 * @date       2025/12/30 15:44
 * @history
*****************************************************************************/
#ifndef CLIENT_CUSTOMIZEEDIT_H
#define CLIENT_CUSTOMIZEEDIT_H

#include <QDebug>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class CustomizeEdit; }
QT_END_NAMESPACE

class CustomizeEdit : public QLineEdit {
Q_OBJECT

public:
    explicit CustomizeEdit(QWidget *parent = nullptr);                      // 构造函数
    ~CustomizeEdit() override;                                              // 析构函数
    void SetMaxLength(int maxLen);                                          // 设置最大长度

protected:
    void focusOutEvent(QFocusEvent *event) override;                        // 重写失去焦点事件

private:
    void limitTextLength(QString text);                                     // 限制文本长度

    int _max_len;

signals:

    void signal_focus_out();
};


#endif //CLIENT_CUSTOMIZEEDIT_H
