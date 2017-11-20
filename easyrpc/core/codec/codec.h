/**
 * @file codec.h
 * @brief 编解码器基类
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include <functional>
#include "easyrpc/core/protocol/protocol_define.h"
#include "easyrpc/utility/protobuf_serialize.h"

class codec
{
public:
    virtual ~codec();
    virtual std::shared_ptr<std::string> encode(int serial_num, 
                                                int func_id,
                                                const std::shared_ptr<google::protobuf::Message>& message);
    virtual void decode(const std::vector<char>& buffer) = 0;
    virtual void reset() = 0;
    virtual void set_decode_data_callback(const std::function<void(const response_body&)>& func);
    std::size_t get_next_recv_bytes();

protected:
    std::size_t next_recv_bytes_ = 0;
    bool decode_header_ = true;
};
