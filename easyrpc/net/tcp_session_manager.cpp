#include "tcp_session_manager.h"

std::shared_ptr<tcp_session> tcp_session_manager::get_session(const std::string& session_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto iter = sessions_.find(session_id);
    if (iter != sessions_.end())
    {
        return iter->second;
    }

    return nullptr;
}

void tcp_session_manager::add_session(const std::string& session_id, 
                                      const std::shared_ptr<tcp_session>& session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.emplace(session_id, session);
}

void tcp_session_manager::remove_session(const std::string& session_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(session_id);
}

void tcp_session_manager::clear()
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.clear();
}
