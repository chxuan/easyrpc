#ifndef _IO_SERVER_POOL_H
#define _IO_SERVER_POOL_H

#include <vector>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace easyrpc
{

class io_service_pool
{
public:
    io_service_pool() = default;
    io_service_pool(io_service_pool&) = delete;
    io_service_pool& operator=(const io_service_pool&) = delete;

    explicit io_service_pool(std::size_t pool_size)
    {
        if (pool_size == 0)
        {
            throw std::runtime_error("io server pool size is 0");
        }
        
        for (std::size_t i = 0; i < pool_size; ++i)
        {
            io_service_ptr ios = std::make_shared<boost::asio::io_service>();
            work_ptr work = std::make_shared<boost::asio::io_service::work>(*ios);
            _ios_vec.emplace_back(ios);
            _work_vec.emplace_back(work);
        }
    }

    void run()
    {
        for (std::size_t i = 0; i < _ios_vec.size(); ++i)
        {
            std::shared_ptr<std::thread> t = 
                std::make_shared<std::thread>(boost::bind(&boost::asio::io_service::run, _ios_vec[i]));
            _thread_vec.emplace_back(t);
        }
    }

    void stop()
    {
        stop_io_services();
        stop_threads();
    }

    boost::asio::io_service& get_io_service()
    {
        boost::asio::io_service& ios = *_ios_vec[_next_io_service];
        ++_next_io_service;
        if (_next_io_service == _ios_vec.size())
        {
            _next_io_service = 0;
        }
        return ios;
    }

private:
    void stop_io_services()
    {
        for (auto& iter : _ios_vec)
        {
            if (iter != nullptr)
            {
                iter->stop();
            }
        }
    }

    void stop_threads()
    {
        for (auto& iter : _thread_vec)
        {
            if (iter != nullptr)
            {
                if (iter->joinable())
                {
                    iter->join();
                }
            }
        }       
    }

private:
    using io_service_ptr = std::shared_ptr<boost::asio::io_service>;
    using work_ptr = std::shared_ptr<boost::asio::io_service::work>;
    std::vector<io_service_ptr> _ios_vec;
    std::vector<work_ptr> _work_vec;
    std::vector<std::shared_ptr<std::thread>> _thread_vec; 
    std::size_t _next_io_service = 0;
};

}

#endif
