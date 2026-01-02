#include "clickedlabel.h"

#include <QMouseEvent>

ClickedLabel::ClickedLabel(QWidget *parent) : QLabel(parent), _curstate(ClickLbState::Normal) {
    this->setCursor(Qt::PointingHandCursor);
}

// 处理鼠标点击事件
void ClickedLabel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (_curstate == ClickLbState::Normal) {
            _curstate = ClickLbState::Selected;
            setProperty("state", _selected_press);
            repolish(this);
            update();
        } else {
            _curstate = ClickLbState::Normal;
            setProperty("state", _normal_press);
            repolish(this);
            update();
        }
        // emit clicked();
        return;
    }
    // 调用基类 mousePressEvent 保证正常的事件处理
    QLabel::mousePressEvent(event);
}

// 处理鼠标悬停进入事件
void ClickedLabel::enterEvent(QEvent *event) {
    if (_curstate == ClickLbState::Normal) {
        setProperty("state", _normal_hover);
        repolish(this);
        update();
    } else {
        setProperty("state", _selected_hover);
        repolish(this);
        update();
    }
    QLabel::enterEvent(event);
}

// 处理鼠标悬停离开事件
void ClickedLabel::leaveEvent(QEvent *event) {
    if (_curstate == ClickLbState::Normal) {
        setProperty("state", _normal);
        repolish(this);
        update();
    } else {
        setProperty("state", _selected);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}

void ClickedLabel::SetState(QString normal, QString hover,
                            QString press, QString select,
                            QString select_hover, QString select_press) {

    _normal = normal;
    _normal_hover = hover;
    _normal_press = press;

    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;
    setProperty("state", normal);
    repolish(this);
}

ClickLbState ClickedLabel::GetCurState() {
    return _curstate;
}

void ClickedLabel::mouseReleaseEvent(QMouseEvent *event) {
    if (Qt::LeftButton == event->button()) {
        if (ClickLbState::Normal == _curstate) {
            setProperty("state", _normal_hover);
            repolish(this);
            update();
        } else {
            setProperty("state", _selected_hover);
            repolish(this);
            update();
        }
        emit clicked(this->text(), _curstate);
    }
    QLabel::mouseReleaseEvent(event);
}

void ClickedLabel::ResetNormalState() {
    _curstate = ClickLbState::Normal;
    setProperty("state", _normal);
    repolish(this);
}

bool ClickedLabel::SetCurState(ClickLbState state) {
    _curstate = state;
    if (_curstate == ClickLbState::Normal) {
        setProperty("state", _normal);
        repolish(this);
    } else if (_curstate == ClickLbState::Selected) {
        setProperty("state", _selected);
        repolish(this);
    }
    return true;
}
