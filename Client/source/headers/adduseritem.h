#ifndef CLIENT_ADDUSERITEM_H
#define CLIENT_ADDUSERITEM_H

#include "listitembase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AddUserItem; }
QT_END_NAMESPACE

class AddUserItem : public ListItemBase {
Q_OBJECT

public:
    explicit AddUserItem(QWidget *parent = nullptr);    // 构造函数
    ~AddUserItem() override;                            // 析构函数
    QSize sizeHint() const override;                    // 返回自定义尺寸

private:
    Ui::AddUserItem *ui;
};


#endif //CLIENT_ADDUSERITEM_H
