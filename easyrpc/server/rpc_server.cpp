#include "rpc_server.h"

rpc_server::~rpc_server()
{

}

void rpc_server::bind(int func_id, const function_t& func)
{
    router_.bind(func_id, func);
}
