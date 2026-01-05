#ifndef CHATSERVERA_USERMANAGER_H
#define CHATSERVERA_USERMANAGER_H

#include "Singleton.h"

#include <mutex>
#include <memory>
#include <unordered_map>

class CSession;

class UserManager : public Singleton<UserManager> {
    friend class Singleton<UserManager>;

public:
    ~UserManager();

    std::shared_ptr<CSession> GetSession(int uid);

    void SetUserSession(int uid, std::shared_ptr<CSession> session);

    void RmvUserSession(int uid, std::string session_id);

private:
    UserManager();

    std::mutex _session_mtx;
    std::unordered_map<int, std::shared_ptr<CSession>> _uid_to_session;
};


#endif //CHATSERVERA_USERMANAGER_H
