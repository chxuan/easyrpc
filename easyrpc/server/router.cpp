#include "router.h"
#include "request.h"
#include "response.h"
#include "easyrpc/utility/logger.h"

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

void router::bind(const std::string& message_name, const request_handler& func)
{
    route_table_.emplace(message_name, func);
}

void router::stop()
{
    threadpool_.stop();
    route_table_.clear();
}

void router::route(const std::shared_ptr<request>& req, const std::shared_ptr<response>& res)
{
    threadpool_.add_task(std::bind(&router::route_thread, this, req, res));
}

void router::route_thread(const std::shared_ptr<request>& req, const std::shared_ptr<response>& res)
{
    auto iter = route_table_.find(req->message->GetDescriptor()->full_name());
    if (iter != route_table_.end())
    {
        try
        {
            iter->second(req, res);
        }
        catch (std::exception& e)
        {
            log_warn << e.what();
        }
    }
    else
    {
        log_warn << "Route failed, message name: " << req->message->GetDescriptor()->full_name();
    }
}
