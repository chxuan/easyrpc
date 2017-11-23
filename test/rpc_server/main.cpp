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

int main()
{
    auto server = std::make_shared<rpc_server>();
    server->set_session_status_callback(std::bind(session_status_callback, 
                                                  std::placeholders::_1, std::placeholders::_2));
    server->listen("0.0.0.0:8888").run();
    log_info() << "rpc server start...";

    std::cin.get();
    return 0;
}

