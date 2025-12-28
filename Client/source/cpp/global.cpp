#include <QCryptographicHash>

#include "global.h"

QString url_prefix = "";

std::function<void(QWidget *)> repolish = [](QWidget *widget) {
    widget->style()->unpolish(widget);      // 去掉旧样式
    widget->style()->polish(widget);        // 加载新样式
};

extern QString md5Encrypt(const QString &input) {
    // 将输入字符转换为字节数组
    QByteArray array = input.toUtf8();
    // 使用md5进行加密
    QByteArray hash = QCryptographicHash::hash(array, QCryptographicHash::Md5);
    // 返回十六进制的加密结果
    return QString(hash.toHex());
}
