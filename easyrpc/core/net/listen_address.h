/**
 * @file listen_address.h
 * @brief tcp服务监听地址
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-19
 */
#pragma once

#include <boost/asio.hpp>

class io_service_pool;

class listen_address
{
public:
    listen_address(std::shared_ptr<io_service_pool>& pool);
    bool listen(const std::string& ip, unsigned short port);
    void accept();

private:
    std::shared_ptr<io_service_pool>& pool_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
