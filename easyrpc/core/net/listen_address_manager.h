/**
 * @file listen_address_manager.h
 * @brief tcp服务监听地址管理
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
class listen_address;

class listen_address_manager
{
public:
    void init_network_address(const std::string& listen_address);
    void init_network_address(const std::vector<std::string>& listen_addresses);
    void init_ios_threads(int num);
    bool start_listen();
    void stop_listen();

private:
    void create_io_service_pool();
    bool listen();
    void accept();

private:
    std::vector<std::string> listen_addresses_string_;
    std::vector<std::shared_ptr<listen_address>> listen_addresses_;
    int ios_threads_ = 4;
    std::shared_ptr<io_service_pool> pool_;
};
