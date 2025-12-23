#include "mainwindow.h"

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QSettings>
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

    // 读取 config.ini 文件
    QString fileName = "config.ini";
    QString appPath = QCoreApplication::applicationDirPath();
    QString path = QDir::toNativeSeparators(appPath + QDir::separator() + fileName);
    QSettings settings(path, QSettings::IniFormat);
    QString host = settings.value("GateServer/host").toString();
    QString port = settings.value("GateServer/port").toString();
    url_prefix = "http://" + host + ":" + port;
    qDebug() << "path: " << path << "\nhost: " << host << "\nport" << port;

    MainWindow w;
    w.show();
    return application.exec();
}