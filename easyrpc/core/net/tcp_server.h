/**
 * @file tcp_server.h
 * @brief tcp服务端
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-16
 */
#pragma once

#include <memory>
#include <functional>
#include "easyrpc/utility/utiltiy.h"
#include "easyrpc/utility/qt_connect.h"

class listen_address_manager;

class tcp_server
{
public:
    tcp_server();
    virtual ~tcp_server();

    tcp_server& listen(const std::string& listen_address);
    tcp_server& listen(const std::vector<std::string>& listen_addresses);
    tcp_server& ios_threads(std::size_t num);
    tcp_server& work_threads(std::size_t num);
    void set_session_status_callback(const std::function<void(bool, const std::string&)>& func);

    virtual bool run();
    virtual void stop();

private slots:
    void handle_session_status_changed(bool established, const std::string& session_id);

private:
    std::size_t work_threads_ = 4;
    std::shared_ptr<listen_address_manager> address_manager_;
    std::function<void(bool, const std::string&)> session_status_callback_ = nullptr;
};
