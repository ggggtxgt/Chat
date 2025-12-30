/******************************************************************************
 * @file       loadingdialog.h
 * @brief      [加载]对话框头文件
 * @author     ggggtxgt
 * @date       2025/12/30 23:12
 * @history
*****************************************************************************/
#ifndef CLIENT_LOADINGDIALOG_H
#define CLIENT_LOADINGDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class LoadingDialog; }
QT_END_NAMESPACE

class LoadingDialog : public QDialog {
Q_OBJECT

public:
    explicit LoadingDialog(QWidget *parent = nullptr);

    ~LoadingDialog() override;

private:
    Ui::LoadingDialog *ui;
};


#endif //CLIENT_LOADINGDIALOG_H
