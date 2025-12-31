#include "chatitembase.h"
#include "bubbleframe.h"

#include <QFont>

ChatItemBase::ChatItemBase(ChatRole role, QWidget *parent) : QWidget(parent), _role(role) {
    _name_label = new QLabel();
    _name_label->setObjectName("chat_user_name");
    QFont font("Microsoft YaHei");
    font.setPointSize(9);
    _name_label->setFont(font);
    _name_label->setFixedHeight(20);
    _icon_label = new QLabel();
    _icon_label->setScaledContents(true);
    _icon_label->setFixedSize(42, 42);
    _bubble_widget = new QWidget();
    QGridLayout *pGLayout = new QGridLayout();
    pGLayout->setVerticalSpacing(3);
    pGLayout->setHorizontalSpacing(3);
    pGLayout->setMargin(3);
    QSpacerItem *pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    if (_role == ChatRole::Self) {
        _name_label->setContentsMargins(0, 0, 8, 0);
        _name_label->setAlignment(Qt::AlignRight);
        pGLayout->addWidget(_name_label, 0, 1, 1, 1);
        pGLayout->addWidget(_icon_label, 0, 2, 2, 1, Qt::AlignTop);
        pGLayout->addItem(pSpacer, 1, 0, 1, 1);
        pGLayout->addWidget(_bubble_widget, 1, 1, 1, 1);
        pGLayout->setColumnStretch(0, 2);
        pGLayout->setColumnStretch(1, 3);
    } else {
        _name_label->setContentsMargins(8, 0, 0, 0);
        _name_label->setAlignment(Qt::AlignLeft);
        pGLayout->addWidget(_icon_label, 0, 0, 2, 1, Qt::AlignTop);
        pGLayout->addWidget(_name_label, 0, 1, 1, 1);
        pGLayout->addWidget(_bubble_widget, 1, 1, 1, 1);
        pGLayout->addItem(pSpacer, 2, 2, 1, 1);
        pGLayout->setColumnStretch(1, 3);
        pGLayout->setColumnStretch(2, 2);
    }
    this->setLayout(pGLayout);
}

ChatItemBase::~ChatItemBase() {}

void ChatItemBase::SetUserName(const QString &name) { _name_label->setText(name); }

void ChatItemBase::SetUserIcon(const QPixmap &icon) { _icon_label->setPixmap(icon); }

void ChatItemBase::SetWidget(QWidget *widget) {
    QGridLayout *layout = (qobject_cast<QGridLayout *>(this->layout()));
    layout->replaceWidget(_bubble_widget, widget);
    delete _bubble_widget;
    _bubble_widget = widget;
}