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
    atimer(boost::asio::io_service& ios) : _timer(ios), _is_single_shot(false) {}
    ~atimer()
    {
        stop();
    }

    void start(std::size_t duration)
    {
        _timer.expires_from_now(Duration(duration));
        _timer.async_wait([this, duration](const boost::system::error_code& ec)
        {
            if (ec)
            {
                return;
            }

            for (auto& func : _func_vec)
            {
                func();
            }

            if (!_is_single_shot)
            {
                start(duration);
            }
        });
    }

    void stop()
    {
        _timer.cancel();
    }

    void bind(const std::function<void()>& func)
    {
        _func_vec.emplace_back(func);
    }

    void set_single_shot(bool is_single_short)
    {
        _is_single_shot = is_single_short; 
    }

private:
    boost::asio::deadline_timer _timer;
    std::function<void()> m_func = nullptr;
    std::vector<std::function<void()>> _func_vec;
    std::atomic<bool> _is_single_shot;
};

}

#endif
