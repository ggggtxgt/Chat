#include "bubbleframe.h"

#include <QDebug>
#include <QPainter>

const int WIDTH_SANJIAO = 8;    // 三角宽

BubbleFrame::BubbleFrame(ChatRole role, QWidget *parent) : QFrame(parent), _role(role), _margin(3) {
    _hb_layout = new QHBoxLayout();
    if (_role == ChatRole::Self)
        _hb_layout->setContentsMargins(_margin, _margin, WIDTH_SANJIAO + _margin, _margin);
    else
        _hb_layout->setContentsMargins(WIDTH_SANJIAO + _margin, _margin, _margin, _margin);

    this->setLayout(_hb_layout);
}

BubbleFrame::~BubbleFrame() {}

void BubbleFrame::SetMargin(int margin) {
    Q_UNUSED(margin)
    _margin = margin;
}

void BubbleFrame::SetWidget(QWidget *widget) {
    if (_hb_layout->count() > 0) { return; }
    else { _hb_layout->addWidget(widget); }
}

void BubbleFrame::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    if (ChatRole::Other == _role) {
        // 画出气泡
        QColor bk_color(Qt::white);
        painter.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(WIDTH_SANJIAO, 0, this->width() - WIDTH_SANJIAO, this->height());
        painter.drawRoundedRect(bk_rect, 5, 5);
        // 画出小三角
        QPointF points[3] = {
                QPointF(bk_rect.x(), 12),
                QPointF(bk_rect.x(), 10 + WIDTH_SANJIAO + 2),
                QPointF(bk_rect.x() - WIDTH_SANJIAO, 10 + WIDTH_SANJIAO - WIDTH_SANJIAO / 2),
        };
        painter.drawPolygon(points, 3);
    } else {
        QColor bk_color(158, 234, 106);
        painter.setBrush(QBrush(bk_color));
        // 画出气泡
        QRect bk_rect = QRect(0, 0, this->width() - WIDTH_SANJIAO, this->height());
        painter.drawRoundedRect(bk_rect, 5, 5);
        // 画出三角
        QPointF points[3] = {
                QPointF(bk_rect.x() + bk_rect.width(), 12),
                QPointF(bk_rect.x() + bk_rect.width(), 12 + WIDTH_SANJIAO + 2),
                QPointF(bk_rect.x() + bk_rect.width() + WIDTH_SANJIAO, 10 + WIDTH_SANJIAO - WIDTH_SANJIAO / 2),
        };
        painter.drawPolygon(points, 3);
    }
    return QFrame::paintEvent(event);
}