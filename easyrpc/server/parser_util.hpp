#ifndef _PARSER_UTIL_H
#define _PARSER_UTIL_H

#include <type_traits>
#include <easypack/easypack.hpp>

namespace easyrpc
{

template<typename... Args>
std::string pack(Args... args)
{
    easypack::pack p;
    p.pack_args(std::forward<Args>(args)...);
    return p.get_string();
}

class parser_util
{
public:
    parser_util() = default;
    parser_util(const parser_util&) = delete;
    parser_util& operator=(const parser_util&) = delete;

    parser_util(const std::string& text) : up_(text) {}

    template<typename T>
    typename std::decay<T>::type get()
    {
        using return_type = typename std::decay<T>::type;
        return_type t;
        up_.unpack_top(t);
        return t;
    }

private:
    easypack::unpack up_;
};

}
#endif
