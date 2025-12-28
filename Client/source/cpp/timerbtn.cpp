#include "timerbtn.h"

#include <QDebug>
#include <QMouseEvent>

TimerBtn::TimerBtn(QWidget *parent) : QPushButton(parent), _counter(10) {
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, [this]() {
        _counter--;
        if (_counter <= 0) {
            _timer->stop();
            _counter = 10;
            this->setText("获取");
            this->setEnabled(true);
            return;
        }
        this->setText(QString::number(_counter));
    });
}

TimerBtn::~TimerBtn() noexcept { _timer->stop(); }

// @todo 即使未输入内容，也可点击[获取按钮]
void TimerBtn::mouseReleaseEvent(QMouseEvent *event) {
    if (Qt::LeftButton == event->button()) {
        // 先调用基类 mouseReleaseEvent 正常处理事件

        // 之后执行自定义的定时逻辑
        QPushButton::mouseReleaseEvent(event);
        qDebug() << "LeftButton was released!";
        this->setEnabled(false);
        this->setText(QString::number(_counter));
        _timer->start(1000);
        emit click();
    }
    // 正常处理鼠标释放事件
    QPushButton::mouseReleaseEvent(event);
}