#include "rpc_server.h"
#include "easyrpc/utility/logger.h"

rpc_server::rpc_server(const std::string& host, int ios_threads, int work_threads)
    : tcp_server(host, ios_threads), 
    work_threads_(work_threads)
{

}

rpc_server::~rpc_server()
{
    stop();
}

bool rpc_server::run()
{
    if (router_.route_table_size() == 0)
    {
        log_warn << "Route table is empty";
        return false;
    }

    if (tcp_server::run())
    {
        router_.run(work_threads_);
        return true;
    }

    return false;
}

void rpc_server::stop()
{
    tcp_server::stop();
    router_.stop();
}

void rpc_server::bind(const std::string& message_name, const request_handler& func)
{
    router_.bind(message_name, func);
}

void rpc_server::deal_request(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp)
{
    router_.route(req, rsp);
}
