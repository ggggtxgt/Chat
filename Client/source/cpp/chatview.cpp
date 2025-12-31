#include "chatview.h"

#include <QEvent>
#include <QPainter>
#include <QScrollBar>
#include <QPaintEvent>
#include <QStyleOption>

ChatView::ChatView(QWidget *parent) : QWidget(parent), _is_append(false) {
    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);
    layout->setMargin(0);

    // 将滚动区域加入垂直布局
    _area = new QScrollArea();
    _area->setObjectName("chat_area");
    layout->addWidget(_area);

    QWidget *w = new QWidget(this);
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);

    // 创建子布局
    QVBoxLayout *pHLayout_1 = new QVBoxLayout();
    pHLayout_1->addWidget(new QWidget(), 100000);
    w->setLayout(pHLayout_1);
    _area->setWidget(w);    // 应该时在QScrollArea构造后执行才对
    /* ************************* 布局设置完成 ************************* */

    _area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar *pVScrollBar = _area->verticalScrollBar();
    connect(pVScrollBar, &QScrollBar::rangeChanged, this, &ChatView::onVScrollBarMoved);

    // 把垂直ScrollBar放到上边 而不是原来的并排
    QHBoxLayout *pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout_2->setMargin(0);
    _area->setLayout(pHLayout_2);
    pVScrollBar->setHidden(true);

    _area->setWidgetResizable(true);
    _area->installEventFilter(this);
    initStyleSheet();
}

ChatView::~ChatView() noexcept {}

void ChatView::AppendChatItem(QWidget *item) {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(_area->widget()->layout());
    layout->insertWidget(layout->count() - 1, item);
    _is_append = true;
}

void ChatView::PrependChatItem(QWidget *item) {}

void ChatView::InsertChatItem(QWidget *item) {}

bool ChatView::eventFilter(QObject *object, QEvent *event) {
    if (QEvent::Enter == event->type() && object == _area) {
        _area->verticalScrollBar()->setHidden(_area->verticalScrollBar()->maximum() == 0);
    } else if (QEvent::Leave == event->type() && object == _area) {
        _area->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(object, event);
}

void ChatView::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void ChatView::initStyleSheet() {
    QScrollBar *scrollBar = _area->verticalScrollBar();
    scrollBar->setStyleSheet("QScrollBar{background:transparent;}"
                             "QScrollBar:vertical{background:transparent;width:8px;}"
                             "QScrollBar::handle:vertical{background:red; border-radius:4px;min-height:20px;}"
                             "QScrollBar::add-line:vertical{height:0px}"
                             "QScrollBar::sub-line:vertical{height:0px}"
                             "QScrollBar::add-page:vertical {background:transparent;}"
                             "QScrollBar::sub-page:vertical {background:transparent;}");
}

void ChatView::onVScrollBarMoved(int min, int max) {
    // 添加item可能调用多次
    if (_is_append) {
        QScrollBar *pVScrollBar = _area->verticalScrollBar();
        pVScrollBar->setSliderPosition(pVScrollBar->maximum());
        // 500毫秒内可能调用多次
        QTimer::singleShot(500, [this]() {
            _is_append = false;
        });
    }
}