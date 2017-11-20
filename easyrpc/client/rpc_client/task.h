/**
 * @file task.h
 * @brief rpc任务类
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include <functional>
#include "crpc/core/protocol/protocol_define.h"

using recv_handler = std::function<void(const response_content&)>; 

struct task
{
    recv_handler handler;
    time_t begin_time;
};
