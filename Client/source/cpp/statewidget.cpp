#include "statewidget.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QStyleOption>

StateWidget::StateWidget(QWidget *parent) : QWidget(parent), _cur_state(ClickLbState::Normal) {
    setCursor(Qt::PointingHandCursor);
    // 添加红点
    AddRedPoint();
}

void StateWidget::paintEvent(QPaintEvent *event) {
    QStyleOption option;
    option.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
    return;
}

void StateWidget::mousePressEvent(QMouseEvent *event) {
    if (Qt::LeftButton == event->button()) {
        if (ClickLbState::Selected == _cur_state) {
            qDebug() << "PressEvent, already to selected press: " << _selected_press;
            QWidget::mousePressEvent(event);
            return;
        }
        if (ClickLbState::Normal == _cur_state) {
            qDebug() << "PressEvent, change to selected press: " << _selected_press;
            _cur_state = ClickLbState::Selected;
            setProperty("state", _selected_press);
            repolish(this);
            update();
        }
        return;
    }
    QWidget::mousePressEvent(event);
}

void StateWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (Qt::LeftButton == event->button()) {
        if (ClickLbState::Normal == _cur_state) {

            setProperty("state", _normal_hover);
            repolish(this);
            update();
        } else {
            setProperty("state", _selected_hover);
            repolish(this);
            update();
        }
        emit clicked();
        return;
    }
    //// 疑问：是否应该调用 mouseReleaseEvent()
    QWidget::mousePressEvent(event);
}

void StateWidget::enterEvent(QMouseEvent *event) {
    if (ClickLbState::Normal == _cur_state) {
        setProperty("state", _normal_hover);
        repolish(this);
        update();
    } else {
        setProperty("state", _selected_hover);
        repolish(this);
        update();
    }
    QWidget::enterEvent(event);
}

void StateWidget::leaveEvent(QMouseEvent *event) {
    if (ClickLbState::Normal == _cur_state) {
        setProperty("state", _normal);
        repolish(this);
        update();
    } else {
        setProperty("state", _selected);
        repolish(this);
        update();
    }
    QWidget::leaveEvent(event);
}

void StateWidget::SetState(QString normal, QString hover, QString press,
                           QString select, QString select_hover, QString select_press) {
    _normal = normal;
    _normal_hover = hover;
    _normal_press = press;

    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;

    setProperty("state", normal);
    repolish(this);
}

ClickLbState StateWidget::GetCurState() { return _cur_state; }

void StateWidget::ClearState() {
    _cur_state = ClickLbState::Normal;
    setProperty("state", _normal);
    repolish(this);
    update();
}

void StateWidget::SetSelected(bool selected) {
    if (selected) {
        _cur_state = ClickLbState::Selected;
        setProperty("state", _selected);
        repolish(this);
        update();
        return;
    }
    _cur_state = ClickLbState::Normal;
    setProperty("state", _normal);
    repolish(this);
    update();
    return;
}

void StateWidget::AddRedPoint() {
    _red_point = new QLabel();
    _red_point->setObjectName("red_point");
    QVBoxLayout *vb_layout = new QVBoxLayout;
    _red_point->setAlignment(Qt::AlignCenter);
    vb_layout->addWidget(_red_point);
    vb_layout->setMargin(0);
    this->setLayout(vb_layout);
    _red_point->setVisible(false);
}

void StateWidget::ShowRedPoint(bool show) {
    _red_point->setVisible(true);
}