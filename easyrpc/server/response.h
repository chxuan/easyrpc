/**
 * @file response.h
 * @brief 包装rpc服务应答参数
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-19
 */
#pragma once

#include "easyrpc/codec/codec.h"
#include "easyrpc/net/tcp_session.h"

struct response
{
    response(const std::shared_ptr<tcp_session>& sess, int serial)
        : session(sess), serial_num(serial) {}

    void set_response(const std::shared_ptr<google::protobuf::Message>& message)
    {
        if (message)
        {
            auto network_data = session->get_codec()->encode(serial_num, message_model::rpc, message);
            if (network_data)
            {
                session->async_write(network_data);
            }
        }
    }

    std::shared_ptr<tcp_session> session;
    int serial_num;
};
