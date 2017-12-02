#include "tcp_server.h"
#include "easyrpc/utility/singletion.h"
#include "easyrpc/codec/codec.h"
#include "easyrpc/net/sig.h"
#include "easyrpc/net/address_listen_manager.h"
#include "easyrpc/net/tcp_session.h"
#include "easyrpc/net/tcp_session_manager.h"

tcp_server::tcp_server()
{
    listen_manager_ = std::make_shared<address_listen_manager>();
    qt_connect(session_status_changed, std::bind(&tcp_server::handle_session_status_changed,
                                             this, std::placeholders::_1, std::placeholders::_2));
}

tcp_server::~tcp_server()
{
    stop();
}

tcp_server& tcp_server::listen(const std::string& address)
{
    listen_manager_->init_network_address(address);
    return *this;
}

tcp_server& tcp_server::listen(const std::vector<std::string>& addresses)
{
    listen_manager_->init_network_address(addresses);
    return *this;
}

tcp_server& tcp_server::ios_threads(int num)
{
    listen_manager_->init_ios_threads(num);
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

void tcp_server::publish(const std::string& session_id, 
                         const std::shared_ptr<google::protobuf::Message>& message)
{
    if (message)
    {
        auto session = singletion<tcp_session_manager>::get_instance().get_session(session_id);
        if (session)
        {
            auto network_data = session->get_codec()->encode(-1, 0, message);
            session->async_write(network_data);
        }
    }
}

bool tcp_server::run()
{
    return listen_manager_->start_listen();
}

void tcp_server::stop()
{
    listen_manager_->stop_listen();
    singletion<tcp_session_manager>::get_instance().clear();
}

void tcp_server::handle_session_status_changed(bool established, const std::string& session_id)
{
    if (!established)
    {
        singletion<tcp_session_manager>::get_instance().remove_session(session_id);
    }

    if (session_status_callback_)
    {
        session_status_callback_(established, session_id);
    }
}
