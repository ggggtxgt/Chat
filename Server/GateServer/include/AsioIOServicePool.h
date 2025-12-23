#ifndef GATESERVER_ASIOIOSERVICEPOOL_H
#define GATESERVER_ASIOIOSERVICEPOOL_H

#include <vector>
#include <boost/asio.hpp>

#include "Singleton.h"

class AsioIOServicePool : public Singleton<AsioIOServicePool> {
    friend class Singleton<AsioIOServicePool>;

public:
    // 将类型重命名
    using IOService = boost::asio::io_context;
    using Work = boost::asio::io_context::work;
    using WorkPtr = std::unique_ptr<Work>;

    ~AsioIOServicePool();                                               // 构造函数
    AsioIOServicePool(const AsioIOServicePool &) = delete;              // 删除拷贝构造
    AsioIOServicePool &operator=(const AsioIOServicePool &) = delete;   // 删除赋值构造

    void Stop();                             // 停止服务池，同时回收资源，唤醒扶起的进程
    boost::asio::io_context &GetIOService(); // 使用轮循的方式返回下一个 io_service

private:
    AsioIOServicePool(std::size_t size = 2);

    // 不支持拷贝的成员变量，应在初始化列表之中
    std::size_t _next_IOService;            // 存储多个 io_context 事件循环，用于处理异步 IO 操作
    std::vector<WorkPtr> _works;            // 存储多个 work 对象，用于阻止 io_context 在没有事件时不会退出
    std::vector<std::thread> _threads;      // 存储运行 io_context::run() 的线程
    std::vector<IOService> _io_services;    // 轮询索引，用于指向下一个 io_context 的下标
};


#endif //GATESERVER_ASIOIOSERVICEPOOL_H
