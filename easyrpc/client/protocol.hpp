#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <string>
#include <type_traits>
#include "base/function_traits.hpp"
#include "easypack/easypack.hpp"

#define EASYRPC_RPC_PROTOCOL_DEFINE(handler, func_type) const static easyrpc::protocol_define<func_type> handler { #handler }

namespace easyrpc
{

template<typename Function>
class protocol_define;

template<typename Return, typename... Args>
class protocol_define<Return(Args...)>
{
public:
    using return_type = typename function_traits<Return(Args...)>::return_type;
    explicit protocol_define(std::string name) : name_(std::move(name)) {}

    std::string pack(Args... args) const
    {
        easypack::pack p;
        p.pack_args(std::forward<Args>(args)...);
        return p.get_string();
    }

    return_type unpack(const std::string& text) const
    {
        easypack::unpack up(text);
        return_type ret;
        up.unpack_args(ret);
        return ret;
    }

    const std::string& name() const
    {
        return name_;
    }
    
private:
    std::string name_;
};

}

#endif
