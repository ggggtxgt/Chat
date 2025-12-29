#ifndef GATESERVER_SINGLETON_H
#define GATESERVER_SINGLETON_H

#include <mutex>
#include <memory>
#include <iostream>

template<typename T>
class Singleton {
public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&] {
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    ~Singleton() { std::cout << "singleton destruct." << std::endl; }

protected:
    Singleton() = default;                                  // 使用默认构造函数
    Singleton(const Singleton<T> &) = delete;               // 删除拷贝构造
    Singleton &operator=(const Singleton<T> &) = delete;    // 删除赋值构造

    static std::shared_ptr<T> _instance;                    // 单例类唯一实例
};

// 初始化静态类对象实例
template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif //GATESERVER_SINGLETON_H
