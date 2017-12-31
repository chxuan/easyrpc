#include "io_service_pool.h"

io_service_pool::io_service_pool(int size)
{
    for (int i = 0; i < size; ++i)
    {
        auto ios = std::make_shared<boost::asio::io_service>();
        auto work = std::make_shared<boost::asio::io_service::work>(*ios);
        ios_pool_.emplace_back(ios);
        work_pool_.emplace_back(work);
    }
}

io_service_pool::~io_service_pool()
{
    stop();
}

void io_service_pool::run()
{
    for (std::size_t i = 0; i < ios_pool_.size(); ++i)
    {
        auto t = std::make_shared<std::thread>(boost::bind(&boost::asio::io_service::run, ios_pool_[i]));
        threads_.emplace_back(t);
    }
}

void io_service_pool::stop()
{
    stop_io_services();
    stop_threads();
}

boost::asio::io_service& io_service_pool::get_io_service()
{
    boost::asio::io_service& ios = *ios_pool_[next_io_service_];
    ++next_io_service_;
    if (next_io_service_ == ios_pool_.size())
    {
        next_io_service_ = 0;
    }

    return ios;
}

void io_service_pool::stop_io_services()
{
    for (auto& ios : ios_pool_)
    {
        if (ios)
        {
            ios->stop();
        }
    }
}

void io_service_pool::stop_threads()
{
    for (auto& t : threads_)
    {
        if (t && t->joinable())
        {
            t->join();
        }
    }       
}
