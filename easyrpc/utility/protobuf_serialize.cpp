#include "protobuf_serialize.h"
#include "easyrpc/utility/logger.h"

std::string protobuf_serialize::serialize(const std::shared_ptr<google::protobuf::Message>& message)
{
    if (!message->IsInitialized())
    {
        log_warn << "message initialized failed";
        return "";
    }

    return message->SerializeAsString();
}

std::shared_ptr<google::protobuf::Message> protobuf_serialize::unserialize(const std::string& message_name, 
                                                                           const std::string& body)
{
    if (message_name.empty())
    {
        log_warn << "message name is empty";
        return nullptr;
    }

    auto message = create_message(message_name);
    if (message == nullptr)
    {
        log_warn<< "message is nullptr";
        return nullptr;
    }

    if (!message->ParseFromString(body))
    {
        log_warn << "parse from string failed, message name: " << message_name;
        return nullptr;
    }

    if (!message->IsInitialized())
    {
        log_warn << "message initialized failed, message name: " << message_name;
        return nullptr;
    }

    return message;
}

std::shared_ptr<google::protobuf::Message> protobuf_serialize::create_message(const std::string& message_name)
{
    const auto descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(message_name);
    if (descriptor)
    {
        const auto prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (prototype)
        {
            return std::shared_ptr<google::protobuf::Message>(prototype->New());            
        }
    }

    return nullptr;
}
