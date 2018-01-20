#include "tcp_session_cache.h"

namespace easyrpc
{

std::shared_ptr<tcp_session> tcp_session_cache::get_session(const std::string& session_id)
{
    lock_shared lock(mutex_, true);
    auto iter = sessions_.find(session_id);
    if (iter != sessions_.end())
    {
        return iter->second;
    }

    return nullptr;
}

void tcp_session_cache::add_session(const std::string& session_id, const std::shared_ptr<tcp_session>& session)
{
    lock_shared lock(mutex_);
    sessions_.emplace(session_id, session);
}

void tcp_session_cache::remove_session(const std::string& session_id)
{
    lock_shared lock(mutex_);
    sessions_.erase(session_id);
}

void tcp_session_cache::clear()
{
    lock_shared lock(mutex_);
    sessions_.clear();
}

}
