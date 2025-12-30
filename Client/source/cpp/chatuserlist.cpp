#include "chatuserlist.h"

ChatUserList::ChatUserList(QWidget *parent) : QListWidget(parent) {
    Q_UNUSED(parent)
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 添加事件过滤器
    this->viewport()->installEventFilter(this);
}

ChatUserList::~ChatUserList() noexcept {}

bool ChatUserList::eventFilter(QObject *watched, QEvent *event) {
    // 检查事件是否为鼠标悬浮进入或离开
    if (this->viewport() == watched) {
        if (QEvent::Enter == event->type()) {
            // 鼠标悬浮，显示滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else {
            // 鼠标离开，隐藏滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    // 检查事件是否为鼠标滚轮事件
    if (this->viewport() == watched && QEvent::Wheel == event->type()) {
        QWheelEvent *wheel = static_cast<QWheelEvent *>(event);
        int numDegress = wheel->y() / 8;
        int numSteps = numDegress / 15; // 计算滚动步数
        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);
        // 检查是否滚动到达底部
        QScrollBar *bar = this->verticalScrollBar();
        int maxScrollValue = bar->maximum();
        int currentValue = bar->value();
        // int pageSize = 10;   // 每页加载的联系人数量

        if (maxScrollValue - currentValue <= 0) {
            // 已经到达底部，加载新的联系人
            qDebug() << "load more chat user.";
            // 发送信号通知聊天界面加载更多聊天内容
            emit signal_loading_chat_user();
        }
        return true;    // 停止事件传递
    }
    return QListWidget::eventFilter(watched, event);
}