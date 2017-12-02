/**
 * @file server_codec.h
 * @brief 服务端编解码器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include "easyrpc/core/codec/codec.h"

class server_codec : public codec
{
protected:
    virtual void handle_decode_data(const packet_body& body, 
                                    const std::shared_ptr<tcp_session>& session);
};
