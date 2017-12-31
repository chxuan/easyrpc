#include "tcp_server.h"
#include "sig.h"
#include "address_listener.h"
#include "tcp_session.h"
#include "tcp_session_manager.h"
#include "easyrpc/utility/singletion.h"
#include "easyrpc/codec/codec.h"

tcp_server::tcp_server()
{
    qt_connect(session_status_changed, std::bind(&tcp_server::deal_session_status_changed, 
                                                 this, std::placeholders::_1, std::placeholders::_2));
}

tcp_server::~tcp_server()
{
    stop();
}

tcp_server& tcp_server::listen(const std::string& host)
{
    host_ = host;
    return *this;
}

tcp_server& tcp_server::ios_threads(int num)
{
    ios_threads_ = num;
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

void tcp_server::publish(const std::string& session_id, const std::shared_ptr<google::protobuf::Message>& message)
{
    if (message)
    {
        auto session = singletion<tcp_session_manager>::get_instance().get_session(session_id);
        if (session)
        {
            auto network_data = session->get_codec()->encode(-1, message);
            session->async_write(network_data);
        }
    }
}

bool tcp_server::run()
{
    listener_ = std::make_shared<address_listener>(host_, ios_threads_);
    return listener_->start_listen();
}

void tcp_server::stop()
{
    if (listener_)
    {
        listener_->stop_listen();
    }
    singletion<tcp_session_manager>::get_instance().clear();
}

void tcp_server::deal_session_status_changed(bool established, const std::string& session_id)
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
