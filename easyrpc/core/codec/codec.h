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
    std::size_t get_next_recv_bytes();

protected:
    template<typename T>
    void copy_from_buffer(T& t, int& pos, const std::vector<char>& buffer)
    {
        memcpy(&t, &buffer[pos], sizeof(t));
        pos += sizeof(t);
    }

    template<typename T>
    void copy_from_buffer(T& t, const std::vector<char>& buffer)
    {
        memcpy(&t, &buffer[0], sizeof(t));
    }

    void copy_from_buffer(std::string& str, int& pos, int len, const std::vector<char>& buffer);

    template<typename T>
    void copy_to_buffer(T& t, std::shared_ptr<std::string>& buffer)
    {
        buffer->append(reinterpret_cast<const char*>(&t), sizeof(t));
    }

    void copy_to_buffer(const std::string& str, std::shared_ptr<std::string>& buffer);

protected:
    int next_recv_bytes_ = 0;
    bool decode_header_ = true;
};
