#ifndef CLIENT_CLICKEDONCELABEL_H
#define CLIENT_CLICKEDONCELABEL_H

#include <QLabel>
#include <QMouseEvent>

class ClickedOnceLabel : public QLabel {
Q_OBJECT
public:
    ClickedOnceLabel(QWidget *parent = nullptr);

    ~ClickedOnceLabel();

    virtual void mouseReleaseEvent(QMouseEvent *ev) override;

signals:

    void clicked(QString);
};

#endif //CLIENT_CLICKEDONCELABEL_H
