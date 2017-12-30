#include "router.h"
#include "request.h"
#include "response.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/net/sig.h"

router::router()
{
    qt_connect(complete_server_decode_data, std::bind(&router::deal_complete_server_decode_data, 
                                                      this, std::placeholders::_1, std::placeholders::_2));
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

void router::bind(const std::string& message_name, const function_t& handler)
{
    route_table_.emplace(message_name, handler);
}

void router::stop()
{
    threadpool_.stop();
    route_table_.clear();
}

void router::deal_complete_server_decode_data(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp)
{
    threadpool_.add_task(std::bind(&router::router_thread, this, req, rsp));
}

void router::router_thread(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp)
{
    auto iter = route_table_.find(req->message()->GetDescriptor()->full_name());
    if (iter != route_table_.end())
    {
        try
        {
            iter->second(req, rsp);
        }
        catch (std::exception& e)
        {
            log_warn << e.what();
        }
    }
    else
    {
        log_warn << "Route failed, message name: " << req->message()->GetDescriptor()->full_name();
    }
}
