#include "tcp_server.h"
#include "easyrpc/core/net/listen_address_manager.h"

tcp_server::tcp_server()
{
    address_manager_ = std::make_shared<listen_address_manager>();
    address_manager_->set_session_status_callback(std::bind(&tcp_server::session_status_callback, this, 
                                                            std::placeholders::_1, std::placeholders::_2));
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

void tcp_server::set_session_status_callback(const std::function<void(bool, const std::string&)>& func)
{
    session_status_callback_ = func;
}

bool tcp_server::run()
{
    return address_manager_->start_listen();
}

void tcp_server::stop()
{
}

void tcp_server::session_status_callback(bool established, const std::string& session_id)
{
    if (session_status_callback_)
    {
        session_status_callback_(established, session_id);
    }
}
