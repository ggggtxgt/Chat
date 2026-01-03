#include "conuseritem.h"
#include "../forms/ui_conuseritem.h"


ConUserItem::ConUserItem(QWidget *parent) : ListItemBase(parent), ui(new Ui::ConUserItem) {
    ui->setupUi(this);
    SetItemType(ListItemType::CONTACT_USER_ITEM);
    ui->red_point->raise();
    ShowRedPoint(false);
}

ConUserItem::~ConUserItem() {
    delete ui;
}

