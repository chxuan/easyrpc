/**
 * @file server_codec.h
 * @brief 服务端编解码器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include "crpc/core/codec/codec.h"

class server_codec : public codec
{
public:
    server_codec();
    virtual ~server_codec();
    virtual void decode(const std::vector<char>& buffer);
    virtual void reset();

private:

};
