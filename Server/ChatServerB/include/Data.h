#ifndef GATESERVER_DATA_H
#define GATESERVER_DATA_H

#include <string>

struct UserInfo {
    int uid;
    std::string pwd;
    std::string name;
    std::string email;
};

#endif //GATESERVER_DATA_H
