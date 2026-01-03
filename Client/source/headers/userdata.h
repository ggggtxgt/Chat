#ifndef CLIENT_USERDATA_H
#define CLIENT_USERDATA_H


#include <QString>

#include <vector>
#include <memory>

class SearchInfo {
public:
    SearchInfo(int uid, QString name, QString nick, QString desc, int sex); // 构造函数
    ~SearchInfo();                                                          // 析构函数

    int _uid;
    int _sex;
    QString _name;
    QString _nick;
    QString _desc;
    QString _icon;
};

class AddFriendApply {
public:
    AddFriendApply(int from_uid, QString name, QString desc,
                   QString icon, QString nick, int sex);

    int _from_uid;
    QString _name;
    QString _desc;
    QString _icon;
    QString _nick;
    int _sex;
};

struct ApplyInfo {
    ApplyInfo(int uid, QString name, QString desc,
              QString icon, QString nick, int sex, int status)
            : _uid(uid), _name(name), _desc(desc),
              _icon(icon), _nick(nick), _sex(sex), _status(status) {}

    ApplyInfo(std::shared_ptr<AddFriendApply> addinfo)
            : _uid(addinfo->_from_uid), _name(addinfo->_name),
              _desc(addinfo->_desc), _icon(addinfo->_icon),
              _nick(addinfo->_nick), _sex(addinfo->_sex),
              _status(0) {}

    void SetIcon(QString head) {
        _icon = head;
    }

    int _uid;
    QString _name;
    QString _desc;
    QString _icon;
    QString _nick;
    int _sex;
    int _status;
};

class TextChatData;

struct AuthInfo {
    AuthInfo(int uid, QString name,
             QString nick, QString icon, int sex) :
            _uid(uid), _name(name), _nick(nick), _icon(icon),
            _sex(sex), _thread_id(0) {}

    void SetChatDatas(std::vector<std::shared_ptr<TextChatData>> _chat_datas);

    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
    int _thread_id;
    std::vector<std::shared_ptr<TextChatData>> _chat_datas;
};

struct AuthRsp {
    AuthRsp(int peer_uid, QString peer_name,
            QString peer_nick, QString peer_icon, int peer_sex)
            : _uid(peer_uid), _name(peer_name), _nick(peer_nick),
              _icon(peer_icon), _sex(peer_sex), _thread_id(0) {
    }

    void SetChatDatas(std::vector<std::shared_ptr<TextChatData>> _chat_datas);

    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
    int _thread_id;
    std::vector<std::shared_ptr<TextChatData>> _chat_datas;
};

struct UserInfo {
    UserInfo(int uid, QString name, QString nick, QString icon, int sex, QString last_msg = "", QString desc = "") :
            _uid(uid), _name(name), _nick(nick), _icon(icon), _sex(sex), _desc(desc) {}

    UserInfo(std::shared_ptr<AuthInfo> auth) :
            _uid(auth->_uid), _name(auth->_name), _nick(auth->_nick),
            _icon(auth->_icon), _sex(auth->_sex), _desc("") {}

    UserInfo(int uid, QString name, QString icon) :
            _uid(uid), _name(name), _icon(icon), _nick(_name),
            _sex(0), _desc("") {
    }

    UserInfo(std::shared_ptr<AuthRsp> auth) :
            _uid(auth->_uid), _name(auth->_name), _nick(auth->_nick),
            _icon(auth->_icon), _sex(auth->_sex), _desc("") {}

    UserInfo(std::shared_ptr<SearchInfo> search_info) :
            _uid(search_info->_uid), _name(search_info->_name), _nick(search_info->_nick),
            _icon(search_info->_icon), _sex(search_info->_sex), _desc(search_info->_desc) {
    }

    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
    QString _desc;
};

struct TextChatData;
struct FriendInfo {
    FriendInfo(int uid, QString name, QString nick, QString icon,
               int sex, QString desc, QString back, QString last_msg=""):_uid(uid),
                                                                         _name(name),_nick(nick),_icon(icon),_sex(sex),_desc(desc),
                                                                         _back(back),_last_msg(last_msg){}

    FriendInfo(std::shared_ptr<AuthInfo> auth_info):_uid(auth_info->_uid),
                                                    _nick(auth_info->_nick),_icon(auth_info->_icon),_name(auth_info->_name),
                                                    _sex(auth_info->_sex){}

    FriendInfo(std::shared_ptr<AuthRsp> auth_rsp):_uid(auth_rsp->_uid),
                                                  _nick(auth_rsp->_nick),_icon(auth_rsp->_icon),_name(auth_rsp->_name),
                                                  _sex(auth_rsp->_sex){}

    void AppendChatMsgs(const std::vector<std::shared_ptr<TextChatData>> text_vec);

    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
    QString _desc;
    QString _back;
    QString _last_msg;
    std::vector<std::shared_ptr<TextChatData>> _chat_msgs;
};

#endif //CLIENT_USERDATA_H
