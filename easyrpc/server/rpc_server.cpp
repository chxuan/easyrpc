#include "rpc_server.h"

rpc_server::~rpc_server()
{

}

void rpc_server::bind(const std::string& id, const function_t& func)
{
    router_.bind(id, func);
}
