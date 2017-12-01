/**
 * @file tcp_session_manager.h
 * @brief tcp会话管理
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-12-01
 */
#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>

class tcp_session;

class tcp_session_manager
{
public:
    std::shared_ptr<tcp_session> get_session(const std::string& session_id);
    void add_session(const std::string& session_id, 
                     const std::shared_ptr<tcp_session>& session);
    void remove_session(const std::string& session_id);
    void clear();

private:
    std::unordered_map<std::string, std::shared_ptr<tcp_session>> sessions_;
    std::mutex mutex_;
};
