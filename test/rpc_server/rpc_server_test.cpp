#include "rpc_server_test.h"
#include "../proto/code/proto_message.pb.h"
#include "easyrpc/easyrpc.h"

rpc_server_test::rpc_server_test()
{
    server_ = std::make_shared<rpc_server>();
    server_->set_session_status_callback(std::bind(&rpc_server_test::session_status_callback, this,
                                                   std::placeholders::_1, std::placeholders::_2));
    server_->register_handler(0x0001, std::bind(&rpc_server_test::echo, this, 
                                                std::placeholders::_1, std::placeholders::_2));
}

void rpc_server_test::run()
{
    bool ok = server_->listen("0.0.0.0:8888").ios_threads(4).work_threads(4).run();
    if (!ok)
    {
        log_error() << "rpc server start failed";
        return;
    }

    log_info() << "rpc server start...";
}

void rpc_server_test::stop()
{
    server_->stop();
    log_info() << "rpc server stoped";
}

void rpc_server_test::session_status_callback(bool established, const std::string& session_id)
{
    if (established)
    {
        log_info() << "session established, session id: " << session_id;
    }
    else 
    {
        log_warn() << "session closed, session id: " << session_id;
    }
}

void rpc_server_test::echo(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp)
{
    log_info() << req->message()->DebugString();
    rsp->set_response(req->message());
}

void rpc_server_test::pushlish_thread()
{
#if 0
    while (!server_stoped)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        if (!session_id.empty())
        {
            auto message = std::make_shared<request_person_info_message>();
            message->set_name("Tom");
            message->set_age(25);

            server->publish(session_id, message);
        }
    }
#endif
}

