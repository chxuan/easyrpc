#include "tcp_server.h"
#include "easyrpc/core/protocol/sig.h"
#include "easyrpc/core/net/listen_address_manager.h"

tcp_server::tcp_server()
{
    address_manager_ = std::make_shared<listen_address_manager>();
    qt_connect(session_status_changed, std::bind(&tcp_server::handle_session_status_changed,
                                             this, std::placeholders::_1, std::placeholders::_2));
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

tcp_server& tcp_server::ios_threads(int num)
{
    address_manager_->init_ios_threads(num);
    return *this;
}

tcp_server& tcp_server::work_threads(int num)
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
    address_manager_->stop_listen();
}

void tcp_server::handle_session_status_changed(bool established, const std::string& session_id)
{
    if (session_status_callback_)
    {
        session_status_callback_(established, session_id);
    }
}
