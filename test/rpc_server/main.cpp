#include "easyrpc/easyrpc.h"

void session_status_callback(bool established, const std::string& session_id)
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

void echo(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp)
{
    log_info() << req->message()->DebugString();
    rsp->set_response(req->message());
}

int main()
{
    auto server = std::make_shared<rpc_server>();
    server->set_session_status_callback(std::bind(session_status_callback, 
                                                  std::placeholders::_1, std::placeholders::_2));
    server->bind(0x0001, std::bind(echo, std::placeholders::_1, std::placeholders::_2));
    bool success = server->listen("0.0.0.0:8888").ios_threads(4).work_threads(4).run();
    if (!success)
    {
        log_error() << "rpc server start failed";
        return 0;
    }

    log_info() << "rpc server start...";

    std::cin.get();
    return 0;
}

