#ifndef CLIENT_APPLYFRIENDLIST_H
#define CLIENT_APPLYFRIENDLIST_H

#include <QEvent>
#include <QListWidget>

class ApplyFriendList : public QListWidget {
Q_OBJECT
public:
    ApplyFriendList(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:

signals:

    void sig_show_search(bool);
};

#endif //CLIENT_APPLYFRIENDLIST_H
