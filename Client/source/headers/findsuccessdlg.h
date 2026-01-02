#ifndef CLIENT_FINDSUCCESSDLG_H
#define CLIENT_FINDSUCCESSDLG_H

#include <memory>

#include <QDialog>

#include "userdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FindSuccessDlg; }
QT_END_NAMESPACE

class FindSuccessDlg : public QDialog {
Q_OBJECT

public:
    explicit FindSuccessDlg(QWidget *parent = nullptr);                 // 构造函数
    ~FindSuccessDlg() override;                                         // 析构函数
    void SetSearchInfo(std::shared_ptr<SearchInfo> si);                 // 设置搜索信息

private:
    Ui::FindSuccessDlg *ui;
    QWidget *_parent;
    std::shared_ptr<SearchInfo> _si;

    private slots:

    void on_add_friend_btn_clicked();
};


#endif //CLIENT_FINDSUCCESSDLG_H
