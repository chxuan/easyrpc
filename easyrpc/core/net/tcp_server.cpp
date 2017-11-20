#include "tcp_server.h"
#include "crpc/core/net/listen_address_manager.h"

tcp_server::tcp_server()
{
    address_manager_ = std::make_shared<listen_address_manager>();
}

tcp_server::~tcp_server()
{
    stop();
}

tcp_server& tcp_server::listen(const std::string& listen_address)
{
    address_manager_->init_network_address(listen_address);
    return *this;
}

tcp_server& tcp_server::listen(const std::vector<std::string>& listen_addresses)
{
    address_manager_->init_network_address(listen_addresses);
    return *this;
}

tcp_server& tcp_server::ios_threads(std::size_t num)
{
    address_manager_->init_ios_threads(num);
    return *this;
}

tcp_server& tcp_server::work_threads(std::size_t num)
{
    work_threads_ = num;
    return *this;
}

bool tcp_server::run()
{
    return address_manager_->start_listen();
}

void tcp_server::stop()
{
}

