#include "rpc_server_test.h"
#include "easyrpc/easyrpc.h"

using namespace std::placeholders;

rpc_server_test::~rpc_server_test()
{
    stop();
}

void rpc_server_test::run()
{
    server_ = std::make_shared<rpc_server>("0.0.0.0:8888", 4, 4);
    server_->set_session_status_callback(std::bind(&rpc_server_test::session_status_callback, this, _1, _2));
    register_handler();

    bool ok = server_->run();
    if (!ok)
    {
        log_error<< "rpc server start failed";
        return;
    }

    pub_thread_ = std::make_shared<std::thread>(std::bind(&rpc_server_test::publish_thread, this));

    log_info << "rpc server start...";
}

void rpc_server_test::stop()
{
    server_stoped_ = true;

    if (pub_thread_ && pub_thread_->joinable())
    {
        pub_thread_->join();
    }

    if (server_)
    {
        server_->stop();
    }
}

void rpc_server_test::register_handler()
{
    server_->bind(echo_message::descriptor()->full_name(), std::bind(&rpc_server_test::echo, this, _1, _2));
}

void rpc_server_test::session_status_callback(bool established, const std::string& session_id)
{
    if (established)
    {
        client_session_id_ = session_id;
        log_info << "session established, session id: " << session_id;
    }
    else 
    {
        log_warn << "session closed, session id: " << session_id;
    }
}

void rpc_server_test::echo(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp)
{
    log_info << req->message()->DebugString();
    rsp->set_response(req->message());
}

void rpc_server_test::publish_thread()
{
    while (!server_stoped_)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if (!client_session_id_.empty())
        {
            server_->send_message(client_session_id_, make_auto_weather());
        }
    }
}

std::shared_ptr<google::protobuf::Message> rpc_server_test::make_auto_weather()
{
    auto message = std::make_shared<auto_weather_message>();
    message->set_city_name("ChengDu");
    message->set_weather("Sunny");

    return message;
}
