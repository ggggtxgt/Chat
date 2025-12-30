#ifndef GATESERVER_LOGICSYSTEM_H
#define GATESERVER_LOGICSYSTEM_H

#include <map>
#include <queue>
#include <thread>
#include <unordered_map>

#include "Data.h"
#include "Const.h"
#include "CSession.h"
#include "Singleton.h"

#include "json/json.h"
#include "json/value.h"
#include "json/reader.h"

typedef std::function<void(std::shared_ptr<CSession>, const short &msg_id, const std::string &msg_data)> FunCallBack;

class LogicSystem : public Singleton<LogicSystem> {
    friend class Singleton<LogicSystem>;

public:
    ~LogicSystem();                                                 // 析构函数
    void PostMsgToQueue(std::shared_ptr<LogicNode> msg);            // 将消息注册到队列
private:
    LogicSystem();                                                  // 构造函数
    void DealMsg();                                                 //
    void RegisterCallBacks();                                       //
    void LoginHandler(std::shared_ptr<CSession>,
                      const short &, const std::string &);          //

private:
    bool _b_stop;
    std::mutex _mutex;
    std::thread _worker_thread;
    std::condition_variable _consume;
    std::map<short, FunCallBack> _func_callbacks;
    std::queue<std::shared_ptr<LogicNode>> _msg_que;
    std::unordered_map<int, std::shared_ptr<UserInfo>> _users;
};


#endif //GATESERVER_LOGICSYSTEM_H
