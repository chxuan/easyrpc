#include "address_listen_manager.h"
#include "easyrpc/utility/utiltiy.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/core/net/io_service_pool.h"
#include "easyrpc/core/net/address_listener.h"

void address_listen_manager::init_network_address(const std::string& address)
{
    addresses_.emplace_back(address);
}

void address_listen_manager::init_network_address(const std::vector<std::string>& addresses)
{
    addresses_ = addresses;
}

void address_listen_manager::init_ios_threads(int num)
{
    ios_threads_ = num;
}

bool address_listen_manager::start_listen()
{
    create_io_service_pool();

    if (listen())
    {
        accept();
        return true;
    }

    return false;
}

void address_listen_manager::stop_listen()
{
    pool_->stop();
    addresses_.clear();
    address_listeners_.clear();
}

void address_listen_manager::create_io_service_pool()
{
    pool_ = std::make_shared<io_service_pool>();
    pool_->init_pool_size(ios_threads_);
    pool_->run();
}

bool address_listen_manager::listen()
{
    for (auto& addr : addresses_)
    {
        std::string ip;
        unsigned short port = 0;
        if (!get_ip_and_port(addr, ip, port))
        {
            log_error() << "parse address failed, address: " << addr;
            return false;
        }

        auto listener = std::make_shared<address_listener>(pool_);
        if (!listener->listen(ip, port))
        {
            return false;
        }

        address_listeners_.emplace_back(listener);
    }

    return true;
}

void address_listen_manager::accept()
{
    for (auto& listener : address_listeners_)
    {
        listener->accept();
    }
}
