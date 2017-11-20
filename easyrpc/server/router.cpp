#include "router.h"

void router::bind(const std::string& id, const function_t& func)
{
    std::lock_guard<std::mutex> lock(mutex_);
    route_table_.emplace(id, func);
}
