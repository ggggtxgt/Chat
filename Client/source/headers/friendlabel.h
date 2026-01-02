#ifndef CLIENT_FRIENDLABEL_H
#define CLIENT_FRIENDLABEL_H

#include <QFrame>


QT_BEGIN_NAMESPACE
namespace Ui { class FriendLabel; }
QT_END_NAMESPACE

class FriendLabel : public QFrame {
Q_OBJECT

public:
    explicit FriendLabel(QWidget *parent = nullptr);                    // 构造
    ~FriendLabel() override;                                            // 析构
    int Width();
    int Height();
    QString Text();
    void SetText(QString text);

private:
    Ui::FriendLabel *ui;
    int _width;
    int _height;
    QString _text;

public slots:

    void slot_close();

signals:

    void signal_close(QString);
};


#endif //CLIENT_FRIENDLABEL_H
