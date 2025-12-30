#include "chatdialog.h"
#include "chatuserwidget.h"
#include "../forms/ui_chatdialog.h"

#include <QAction>
#include <QRandomGenerator>

ChatDialog::ChatDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::ChatDialog), _mode(ChatUIMode::ChatMode),
        _state(ChatUIMode::ChatMode), _b_loading(false) {
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
        showSearch(false);
    });
    showSearch(false);

    AddChatUserList();
}

ChatDialog::~ChatDialog() {
    delete ui;
}

void ChatDialog::showSearch(bool b_search) {
    if (b_search) {
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    } else if (ChatUIMode::ChatMode == _state) {
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        _mode = ChatUIMode::ChatMode;
    } else if (ChatUIMode::ContactMode == _state) {
        ui->chat_user_list->hide();
        ui->con_user_list->show();
        ui->search_list->hide();
        _mode = ChatUIMode::ContactMode;
    }
}

// *************************************************************************************************
//// 测试数据
std::vector<QString> strs = {"hello world !",
                             "nice to meet u",
                             "New year，new life",
                             "You have to love yourself",
                             "My love is written in the wind ever since the whole world is you"};

std::vector<QString> heads = {
        ":/img/head_1.jpg",
        ":/img/head_2.jpg",
        ":/img/head_3.jpg",
        ":/img/head_4.jpg",
        ":/img/head_5.jpg"
};

std::vector<QString> names = {
        "wm",
        "lly",
        "ggggtxgt",
        "kwwl",
        "ckel",
        "ylck",
        "xyon",
        "stal"
};
// *************************************************************************************************

void ChatDialog::AddChatUserList() {
    // 创建QListWidgetItem，并设置自定义的widget
    for (int i = 0; i < 13; i++) {
        // 生成0到99之间的随机整数(从而使得头像随机、消息随机)
        int randomValue = QRandomGenerator::global()->bounded(100);
        int str_i = randomValue % strs.size();
        int head_i = randomValue % heads.size();
        int name_i = randomValue % names.size();

        auto *chat_user_wid = new ChatUserWidget();
        chat_user_wid->SetInfo(names[name_i], heads[head_i], strs[str_i]);
        QListWidgetItem *item = new QListWidgetItem;
        // qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chat_user_wid);
    }
}
