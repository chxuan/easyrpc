#include "listen_address_manager.h"
#include "easyrpc/utility/utiltiy.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/core/net/io_service_pool.h"
#include "easyrpc/core/net/listen_address.h"

void listen_address_manager::init_network_address(const std::string& listen_address)
{
    listen_addresses_string_.emplace_back(listen_address);
}

void listen_address_manager::init_network_address(const std::vector<std::string>& listen_addresses)
{
    listen_addresses_string_ = listen_addresses;
}

void listen_address_manager::init_ios_threads(std::size_t num)
{
    ios_threads_ = num;
}

bool listen_address_manager::start_listen()
{
    pool_ = std::make_shared<io_service_pool>();
    pool_->init_pool_size(ios_threads_);
    pool_->run();

    if (listen())
    {
        accept();
        return true;
    }

    return false;
}

bool listen_address_manager::listen()
{
    for (auto& in_address : listen_addresses_string_)
    {
        std::string ip;
        unsigned short port = 0;
        if (!get_ip_and_port(in_address, ip, port))
        {
            log_error() << "parse address failed, address: " << in_address;
            return false;
        }

        auto address = std::make_shared<listen_address>(pool_);
        if (!address->listen(ip, port))
        {
            return false;
        }

        listen_addresses_.emplace_back(address);
    }

    return true;
}

void listen_address_manager::accept()
{
    for (auto& address : listen_addresses_)
    {
        address->accept();
    }
}