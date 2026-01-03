#ifndef CLIENT_CONUSERITEM_H
#define CLIENT_CONUSERITEM_H

#include "userdata.h"
#include "listitembase.h"

#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class ConUserItem; }
QT_END_NAMESPACE

class ConUserItem : public ListItemBase {
Q_OBJECT

public:
    explicit ConUserItem(QWidget *parent = nullptr);

    ~ConUserItem();

    QSize sizeHint() const override;

    void SetInfo(std::shared_ptr<AuthInfo> auth_info);
    void SetInfo(std::shared_ptr<AuthRsp> auth_rsp);

    void SetInfo(int uid, QString name, QString icon);

    void ShowRedPoint(bool show = false);

    std::shared_ptr<UserInfo> GetInfo();

private:
    Ui::ConUserItem *ui;
    std::shared_ptr<UserInfo> _info;
};

#endif //CLIENT_CONUSERITEM_H
