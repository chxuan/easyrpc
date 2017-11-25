#include "router.h"

void router::bind(int func_id, const function_t& func)
{
    std::lock_guard<std::mutex> lock(mutex_);
    route_table_.emplace(func_id, func);
}
