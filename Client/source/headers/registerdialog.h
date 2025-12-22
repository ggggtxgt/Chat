/******************************************************************************
 * @file       registerdialog.h
 * @brief      注册界面头文件
 * @author     ggggtxgt
 * @date       2025/12/22 13:48
 * @history
*****************************************************************************/
#ifndef CLIENT_REGISTERDIALOG_H
#define CLIENT_REGISTERDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class RegisterDialog; }
QT_END_NAMESPACE

class RegisterDialog : public QDialog {
Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

    ~RegisterDialog() override;

private:
    Ui::RegisterDialog *ui;
};


#endif //CLIENT_REGISTERDIALOG_H
