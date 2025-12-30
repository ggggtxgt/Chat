#include "listitembase.h"

#include <QPainter>
#include <QStyleOption>

ListItemBase::ListItemBase(QWidget *parent) : QWidget(parent) {}

ListItemBase::~ListItemBase() noexcept {}

void ListItemBase::SetItemType(ListItemType type) { _type = type; }

ListItemType ListItemBase::GetItemType() { return _type; }

void ListItemBase::paintEvent(QPaintEvent *event) {
    QStyleOption option;
    option.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}