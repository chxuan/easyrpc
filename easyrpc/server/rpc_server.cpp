#include "rpc_server.h"

rpc_server::~rpc_server()
{
    stop();
}

bool rpc_server::run()
{
    if (router_.route_table_size() == 0)
    {
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

void rpc_server::bind(int func_id, const function_t& handler)
{
    router_.bind(func_id, handler);
}
