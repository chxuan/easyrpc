/**
 * @file io_service_pool.h
 * @brief io service池
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <vector>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

class io_service_pool
{
public:
    io_service_pool(int size);
    ~io_service_pool();

    void run();
    void stop();
    boost::asio::io_service& get_io_service();

private:
    void stop_io_services();
    void stop_threads();

private:
    std::vector<std::shared_ptr<boost::asio::io_service>> ios_pool_;
    std::vector<std::shared_ptr<boost::asio::io_service::work>> work_pool_;
    std::vector<std::shared_ptr<std::thread>> threads_; 
    std::size_t next_io_service_ = 0;
};
