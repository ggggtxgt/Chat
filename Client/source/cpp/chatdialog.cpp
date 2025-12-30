#include "chatdialog.h"
#include "../forms/ui_chatdialog.h"

#include <QAction>

ChatDialog::ChatDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    ui->add_btn->SetState("normal", "hover", "press");
    ui->search_edit->SetMaxLength(15);

    // 添加[搜索]放大镜图标，默认文本搜索
    QAction *searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/img/search.png"));
    ui->search_edit->addAction(searchAction, QLineEdit::LeadingPosition);
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索"));

    // 创建清除动作并设置图标（即：X号），初始时使用透明图片
    QAction *clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/img/close_transparent.png"));

    // 将清除图标放在 LineEdit 的末尾位置
    ui->search_edit->addAction(clearAction, QLineEdit::TrailingPosition);

    // 当需要显示清除图标时，更改为实际的清除图标
    connect(ui->search_edit, &QLineEdit::textChanged, [clearAction](const QString &text) {
        if (!text.isEmpty()) {
            clearAction->setIcon(QIcon(":/img/close_search.png"));
        } else {
            // 文本为空，切换为透明图片
            clearAction->setIcon(QIcon(":/img/close_transparent.png"));
        }
    });

    // 连接清除动作的触发信号到槽函数，用于清除文本
    connect(clearAction, &QAction::triggered, [this, clearAction]() {
        ui->search_edit->clear();
        // 清空文本之后，切换为透明图片
        clearAction->setIcon(QIcon(":/img/close_transparent.png"));
        ui->search_edit->clearFocus();
        // 清除按钮被按下则不需要搜索框
        // ShowSearch(false);
    });
}

ChatDialog::~ChatDialog() {
    delete ui;
}
