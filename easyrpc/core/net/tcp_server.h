/**
 * @file tcp_server.h
 * @brief tcp服务端
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-16
 */
#pragma once

#include <memory>
#include "crpc/utility/utiltiy.h"

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

    virtual bool run();
    virtual void stop();

protected:

private:
    std::size_t work_threads_ = 4;
    std::shared_ptr<listen_address_manager> address_manager_;
};
