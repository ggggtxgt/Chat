#include "mainwindow.h"

#include <QFile>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    // 加载 qss 文件
    QFile qss("../res/style/stylesheet.qss");
    if (qss.open(QFile::ReadOnly)) {
        qDebug() << "open qss success.";
        QString style = QLatin1String(qss.readAll());
        application.setStyleSheet(style);
        qss.close();
    } else {
        qDebug() << "open qss failed.";
    }

    MainWindow w;
    w.show();
    return application.exec();
}