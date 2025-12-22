/******************************************************************************
 * @file       singleton.h
 * @brief      单例模板基类
 * @author     ggggtxgt
 * @date       2025/12/22 18:13
 * @history
*****************************************************************************/
#ifndef CLIENT_SINGLETON_H
#define CLIENT_SINGLETON_H

#include "global.h"

template<typename T>
class Singleton {
public:
    // 使用 call_once 确保安全
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    ~Singleton() {
        std::cout << "single destruct." << std::endl;
    }

protected:
    Singleton() = default;                              // 使用默认构造
    Singleton(const Singleton<T> &) = delete;           // 删除拷贝构造
    Singleton &operator=(const Singleton<T> &) = delete;// 删除赋值构造

    // 单例类的唯一实例，使用静态成员变量确保整个程序生命周期中只有一个实例
    static std::shared_ptr<T> _instance;
};

// 类的静态成员必须被初始化
// 若为模板类，在类外初始化，否则可在类内初始化
template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif //CLIENT_SINGLETON_H
