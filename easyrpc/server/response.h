/**
 * @file response.h
 * @brief 包装rpc服务应答参数
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-19
 */
#pragma once

#include <memory>
#include <google/protobuf/message.h>

class tcp_session;

class response
{
public:
    response(const std::shared_ptr<tcp_session>& session, int serial_num);

    void set_response(const std::shared_ptr<google::protobuf::Message>& message);

private:
    std::shared_ptr<tcp_session> session_;
    int serial_num_;
};
