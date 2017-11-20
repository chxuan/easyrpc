/**
 * @file result.hpp
 * @brief 存放返回结果
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <google/protobuf/message.h>
#include "easyrpc/core/protocol/protocol_define.h"

class result
{
public:
    result(const error_code& ec, 
           int call_id, 
           const std::shared_ptr<google::protobuf::Message>& message = nullptr);

    std::shared_ptr<google::protobuf::Message> message() const;
    int serial_num() const;
    error_code error() const;

private:
    error_code ec_;
    int serial_num_ = 0;
    std::shared_ptr<google::protobuf::Message> message_ = nullptr;
};

