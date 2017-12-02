/**
 * @file address_listen_manager.h
 * @brief tcp服务地址监听管理者
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-16
 */
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

class io_service_pool;
class address_listener;

class address_listen_manager
{
public:
    void init_network_address(const std::string& address);
    void init_network_address(const std::vector<std::string>& addresses);
    void init_ios_threads(int num);
    bool start_listen();
    void stop_listen();

private:
    void create_io_service_pool();
    bool listen();
    void accept();

private:
    std::vector<std::string> addresses_;
    std::vector<std::shared_ptr<address_listener>> address_listeners_;
    int ios_threads_ = 4;
    std::shared_ptr<io_service_pool> pool_ = nullptr;
};
