/**
 * @file result.hpp
 * @brief 存放返回结果
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include "easyrpc/codec/codec.h"

struct result
{
    result(int serial, message_model mod, const std::shared_ptr<google::protobuf::Message>& msg)
        : serial_num(serial), model(mod), message(msg) {}
    int serial_num;
    message_model model;
    std::shared_ptr<google::protobuf::Message> message;
};

