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
            ios_vec_.emplace_back(ios);
            work_vec_.emplace_back(work);
        }
    }

    void run()
    {
        for (std::size_t i = 0; i < ios_vec_.size(); ++i)
        {
            thread_ptr t = std::make_shared<std::thread>(boost::bind(&boost::asio::io_service::run, ios_vec_[i]));
            thread_vec_.emplace_back(t);
        }
    }

    void stop()
    {
        stop_io_services();
        stop_threads();
    }

    boost::asio::io_service& get_io_service()
    {
        boost::asio::io_service& ios = *ios_vec_[next_io_service_];
        ++next_io_service_;
        if (next_io_service_ == ios_vec_.size())
        {
            next_io_service_ = 0;
        }
        return ios;
    }

private:
    void stop_io_services()
    {
        for (auto& iter : ios_vec_)
        {
            if (iter != nullptr)
            {
                iter->stop();
            }
        }
    }

    void stop_threads()
    {
        for (auto& iter : thread_vec_)
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
    using thread_ptr = std::shared_ptr<std::thread>;
    std::vector<io_service_ptr> ios_vec_;
    std::vector<work_ptr> work_vec_;
    std::vector<thread_ptr> thread_vec_; 
    std::size_t next_io_service_ = 0;
};

}

#endif
