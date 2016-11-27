#ifndef _SCOPE_GUARD_H
#define _SCOPE_GUARD_H

#include <iostream>

namespace easyrpc
{

template<typename T>
class scope_guard
{
public:
    scope_guard() = default;
    scope_guard(const scope_guard&) = delete;
    scope_guard& operator=(const scope_guard&) = delete;

    explicit scope_guard(T&& func) : func_(std::move(func)) {}
    explicit scope_guard(const T& func) : func_(func) {}

    ~scope_guard()
    {
        if (!dismiss_)
        {
            func_();
        }
    }

    scope_guard(scope_guard&& other)
        : func_(std::move(other.func_))
    {
        other.dismiss();
    }

    void dismiss()
    {
        dismiss_ = true;
    }

private:
    T func_;
    bool dismiss_ = false;
};

template<typename T>
scope_guard<typename std::decay<T>::type> make_guard(T&& func)
{
    return scope_guard<typename std::decay<T>::type>(std::forward<T>(func));
}

}

#endif
