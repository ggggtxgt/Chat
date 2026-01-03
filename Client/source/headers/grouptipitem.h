#ifndef CLIENT_GROUPTIPITEM_H
#define CLIENT_GROUPTIPITEM_H

#include <QWidget>
#include "listitembase.h"

namespace Ui {
    class GroupTipItem;
}

class GroupTipItem : public ListItemBase {
Q_OBJECT

public:
    explicit GroupTipItem(QWidget *parent = nullptr);

    ~GroupTipItem();

    QSize sizeHint() const override;

    void SetGroupTip(QString str);

private:
    QString _tip;
    Ui::GroupTipItem *ui;
};


#endif //CLIENT_GROUPTIPITEM_H
