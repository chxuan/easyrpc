/**
 * @file protobuf_serialize.h
 * @brief protobuf序列化/反序列化类
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <memory>
#include <google/protobuf/message.h>

class protobuf_serialize
{
public:
    static std::string serialize(const std::shared_ptr<google::protobuf::Message>& message);
    static std::shared_ptr<google::protobuf::Message> unserialize(const std::string& message_name,
                                                                  const std::string& body);
    static bool is_vaild_message(const std::shared_ptr<google::protobuf::Message>& message);

private:
    static std::shared_ptr<google::protobuf::Message> create_message(const std::string& message_name);
};
