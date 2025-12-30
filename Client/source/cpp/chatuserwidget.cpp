#include "chatuserwidget.h"
#include "../forms/ui_chatuserwidget.h"

ChatUserWidget::ChatUserWidget(QWidget *parent) :
        ListItemBase(parent),
        ui(new Ui::ChatUserWidget) {
    ui->setupUi(this);
    SetItemType(ListItemType::CHAT_USER_ITEM);
}

ChatUserWidget::~ChatUserWidget() noexcept { delete ui; }

QSize ChatUserWidget::sizeHint() const {
    return QSize(250, 70);  // 返回自定义尺寸
}

void ChatUserWidget::SetInfo(QString name, QString head, QString msg) {
    _name = name;
    _head = head;
    _msg = msg;
    // 加载图片
    QPixmap pixmap(_head);

    // 设置图片自动缩放
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->user_name_lb->setText(_name);
    ui->user_chat_lb->setText(_msg);
}