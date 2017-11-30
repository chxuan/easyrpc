/**
 * @file task.h
 * @brief rpc任务类
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include <functional>
#include <memory>
#include <google/protobuf/message.h>

class result;

using result_handler = std::function<void(const std::shared_ptr<result>&)>; 
using sub_handler = std::function<void(const std::shared_ptr<google::protobuf::Message>&)>;

struct task
{
    result_handler handler;
    time_t begin_time;
};
