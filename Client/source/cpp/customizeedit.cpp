#include "customizeedit.h"

CustomizeEdit::CustomizeEdit(QWidget *parent) : QLineEdit(parent), _max_len(0) {
    connect(this, &QLineEdit::textChanged, this, &CustomizeEdit::limitTextLength);
}

CustomizeEdit::~CustomizeEdit() noexcept {}

void CustomizeEdit::SetMaxLength(int maxLen) { _max_len = maxLen; }

void CustomizeEdit::focusOutEvent(QFocusEvent *event) {
    // 调用基类 focusOutEvent() 函数，保证基类的行为正确执行
    QLineEdit::focusOutEvent(event);
    // 发送失去焦点的信号
    emit signal_focus_out();
}

void CustomizeEdit::limitTextLength(QString text) {
    if (_max_len <= 0) { return; }
    QByteArray array = text.toUtf8();
    if (array.size() > _max_len) {
        array = array.left(_max_len);
        this->setText(QString::fromUtf8(array));
    }
}