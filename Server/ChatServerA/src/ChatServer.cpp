#include <mutex>
#include <thread>
#include <csignal>

#include "Glog.h"
#include "CServer.h"
#include "LogicSystem.h"
#include "ConfigManager.h"
#include "AsioIOServicePool.h"

bool b_stop = false;
std::mutex mutex_quit;
std::condition_variable cond_quit;

int main(int argc, char *argv[]) {
    Glog::GetInstance()->InitLog(argv[0]);
    try {
        auto &cfg = ConfigManager::Inst();
        auto pool = AsioIOServicePool::GetInstance();
        boost::asio::io_context io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&io_context, pool](auto, auto) {
            io_context.stop();
            pool->Stop();
        });
        auto port_str = cfg["SelfServer"]["Port"];
        CServer server(io_context, atoi(port_str.c_str()));
        io_context.run();
    } catch (std::exception &exp) {
        LOG(ERROR) << "Exception: " << exp.what();
    }
}