#include <iostream>

#include "Glog.h"
#include "AsioIOServicePool.h"

AsioIOServicePool::AsioIOServicePool(std::size_t size) : _io_services(size),
                                                         _works(size), _next_IOService(0) {
    // 绑定 io_context 和 work，使得即使没有事件发生，io_service也不会退出
    // 轮询 io_context 下载，创建 boost::asio::io_context::work 实例
    for (std::size_t i = 0; i < size; i++) {
        _works[i] = std::unique_ptr<Work>(new Work(_io_services[i]));
    }

    // 遍历 io_service，创建多个线程，并且启动 io_service
    for (std::size_t i = 0; i < _io_services.size(); i++) {
        _threads.emplace_back([this, i]() {
            _io_services[i].run();
        });
    }
}

AsioIOServicePool::~AsioIOServicePool() {
    Stop();
    LOG(INFO) << "AsioIOServicePool destruct!";
}

boost::asio::io_context &AsioIOServicePool::GetIOService() {
    // 通过轮询的方式较为平均地使用每个 io_service
    auto &service = _io_services[_next_IOService++];
    if (_next_IOService == _io_services.size()) {
        _next_IOService = 0;
    }
    return service;
}

void AsioIOServicePool::Stop() {
    // 由于只执行 work.reset() 不能使用 io_context 从运行状态退出
    // 当 io_context 已经绑定读或写的监听事件之后，需要手动关闭该服务
    for (auto &work: _works) {
        work->get_io_context().stop();
        work.reset();
    }
    // 等待线程退出
    for (auto &thread: _threads) {
        thread.join();
    }
}