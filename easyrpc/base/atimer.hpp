#ifndef _ATIMER_H
#define _ATIMER_H

#include <vector>
#include <atomic>
#include <functional>
#include <boost/timer.hpp>
#include <boost/asio.hpp>

namespace easyrpc
{

template<typename Duration = boost::posix_time::milliseconds>
class atimer
{
public:
    atimer() = default;
    atimer(boost::asio::io_service& ios) : timer_(ios), is_single_shot_(false) {}
    ~atimer()
    {
        stop();
    }

    void start(std::size_t duration)
    {
        timer_.expires_from_now(Duration(duration));
        timer_.async_wait([this, duration](const boost::system::error_code& ec)
        {
            if (ec)
            {
                return;
            }

            for (auto& func : func_vec_)
            {
                func();
            }

            if (!is_single_shot_)
            {
                start(duration);
            }
        });
    }

    void stop()
    {
        timer_.cancel();
    }

    void bind(const std::function<void()>& func)
    {
        func_vec_.emplace_back(func);
    }

    void set_single_shot(bool is_single_short)
    {
        is_single_shot_ = is_single_short; 
    }

private:
    boost::asio::deadline_timer timer_;
    std::vector<std::function<void()>> func_vec_;
    std::atomic<bool> is_single_shot_;
};

}

#endif
