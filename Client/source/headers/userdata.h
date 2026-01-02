#ifndef CLIENT_USERDATA_H
#define CLIENT_USERDATA_H


#include <QString>

class SearchInfo {
public:
    SearchInfo(int uid, QString name, QString nick, QString desc, int sex); // 构造函数
    ~SearchInfo();                                                          // 析构函数

    int _uid;
    int _sex;
    QString _name;
    QString _nick;
    QString _desc;
};


#endif //CLIENT_USERDATA_H
