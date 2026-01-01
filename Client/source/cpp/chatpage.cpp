#include "chatpage.h"
#include "textbubble.h"
#include "chatitembase.h"
#include "picturebubble.h"
#include "messagetextedit.h"
#include "../forms/ui_chatpage.h"


#include <QPainter>
#include <QStyleOption>
#include <QShortcut>

ChatPage::ChatPage(QWidget *parent) : QWidget(parent), ui(new Ui::ChatPage) {
    ui->setupUi(this);
    // 设置按钮样式
    ui->receive_btn->SetState("normal", "hover", "press");
    ui->send_btn->SetState("normal", "hover", "press");

    // 设置图标样式
    ui->emo_lb->SetState("normal", "hover", "press", "normal", "hover", "press");
    ui->file_lb->SetState("normal", "hover", "press", "normal", "hover", "press");
}

ChatPage::~ChatPage() {
    delete ui;
}

void ChatPage::printEvent(QPaintEvent *event) {
    QStyleOption option;
    option.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

void ChatPage::on_send_btn_clicked() {
    auto pTextEdit = ui->chatEdit;
    ChatRole role = ChatRole::Self;
    QString userName = QStringLiteral("wm");
    QString userIcon = ":/img/head_1.jpg";

    const QVector<MsgInfo> &msgList = pTextEdit->GetMsgList();
    for (int i = 0; i < msgList.size(); ++i) {
        QString type = msgList[i].msg_flag;
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->SetUserName(userName);
        pChatItem->SetUserIcon(QPixmap(userIcon));
        QWidget *pBubble = nullptr;
        if (type == "text") {
            pBubble = new TextBubble(role, msgList[i].content);
        } else if (type == "image") {
            pBubble = new PictureBubble(QPixmap(msgList[i].content), role);
        } else if (type == "file") {

        }
        if (pBubble != nullptr) {
            pChatItem->SetWidget(pBubble);
            ui->chat_data_list->AppendChatItem(pChatItem);
        }
    }
}
