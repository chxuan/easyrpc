#include "router.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/net/sig.h"
#include "easyrpc/server/request.h"
#include "easyrpc/server/response.h"

router::router()
{
    qt_connect(complete_server_decode_data, std::bind(&router::handle_complete_server_decode_data, this, 
                                                      std::placeholders::_1, 
                                                      std::placeholders::_2,
                                                      std::placeholders::_3));
}

router::~router()
{
    stop();
}

void router::run(int work_threads)
{
    threadpool_.init_thread_size(work_threads);
}

std::size_t router::route_table_size()
{
    return route_table_.size();
}

void router::register_handler(int func_id, const function_t& handler)
{
    route_table_.emplace(func_id, handler);
}

void router::stop()
{
    threadpool_.stop();
    route_table_.clear();
}

void router::handle_complete_server_decode_data(int func_id, 
                                                const std::shared_ptr<request>& req,
                                                const std::shared_ptr<response>& rsp)
{
    threadpool_.add_task(std::bind(&router::router_thread, this, func_id, req, rsp));
}

void router::router_thread(int func_id, 
                           const std::shared_ptr<request>& req, 
                           const std::shared_ptr<response>& rsp)
{
    auto iter = route_table_.find(func_id);
    if (iter != route_table_.end())
    {
        try
        {
            iter->second(req, rsp);
        }
        catch (std::exception& e)
        {
            log_warn() << e.what();
        }
    }
    else
    {
        log_warn() << "route failed, func id: " << func_id;
    }
}
