/**
 * @file request.h
 * @brief 包装rpc服务请求参数
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-19
 */
#pragma once

#include <google/protobuf/message.h>

namespace easyrpc
{

struct request
{
    request(const std::shared_ptr<google::protobuf::Message>& msg, const std::string& id)
        : message(msg), session_id(id) {}

    std::shared_ptr<google::protobuf::Message> message;
    std::string session_id;
};

}
