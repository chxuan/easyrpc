/**
 * @file request.h
 * @brief 包装rpc服务请求参数
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-19
 */
#pragma once

#include <google/protobuf/message.h>

class request
{
public:
    request(const std::shared_ptr<google::protobuf::Message>& message, const std::string& session_id);

    std::shared_ptr<google::protobuf::Message> message() const;
    std::string session_id() const;

private:
    std::shared_ptr<google::protobuf::Message> message_;
    std::string session_id_;
};
