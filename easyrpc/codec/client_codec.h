/**
 * @file client_codec.h
 * @brief 客户端编解码器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include "codec.h"

namespace easyrpc
{

class result;

using result_handler = std::function<void(const std::shared_ptr<result>&)>;

class client_codec : public codec
{
public:
    client_codec(const result_handler& func);

protected:
    virtual void deal_decode_data(const packet_body& body, const std::shared_ptr<tcp_session>& session);

private:
    result_handler func_;
};

}
