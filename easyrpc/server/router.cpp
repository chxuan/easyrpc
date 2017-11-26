#include "router.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/core/protocol/sig.h"
#include "easyrpc/server/request.h"
#include "easyrpc/server/response.h"

router::router()
{
    qt_connect(complete_server_decode_data, std::bind(&router::handle_complete_server_decode_data, this, 
                                                      std::placeholders::_1, 
                                                      std::placeholders::_2,
                                                      std::placeholders::_3));
}

void router::bind(int func_id, const function_t& func)
{
    std::lock_guard<std::mutex> lock(mutex_);
    route_table_.emplace(func_id, func);
}

void router::handle_complete_server_decode_data(int func_id, 
                                                const std::shared_ptr<request>& req,
                                                const std::shared_ptr<response>& rsp)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto iter = route_table_.find(func_id);
    if (iter != route_table_.end())
    {
        (void)req;
        (void)rsp;
    }
    else
    {
        log_warn() << "Route failed, func id: " << func_id;
    }
}
