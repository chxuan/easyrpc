/**
 * @file result.hpp
 * @brief 存放返回结果
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <google/protobuf/message.h>

class result
{
public:
    result(int serial_num, const std::shared_ptr<google::protobuf::Message>& message);

    std::shared_ptr<google::protobuf::Message> message() const;
    int serial_num() const;

private:
    int serial_num_;
    std::shared_ptr<google::protobuf::Message> message_;
};

