#include "chatdialog.h"
#include "../forms/ui_chatdialog.h"


ChatDialog::ChatDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    ui->add_btn->SetState("normal", "hover", "press");
    ui->search_edit->SetMaxLength(15);
}

ChatDialog::~ChatDialog() {
    delete ui;
}
