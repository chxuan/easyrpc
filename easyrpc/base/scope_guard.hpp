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

    explicit scope_guard(T&& func) : _func(std::move(func)) {}
    explicit scope_guard(const T& func) : _func(func) {}

    ~scope_guard()
    {
        if (!_dismiss)
        {
            _func();
        }
    }

    scope_guard(scope_guard&& other)
        : _func(std::move(other._func))
    {
        other.dismiss();
    }

    void dismiss()
    {
        _dismiss = true;
    }

private:
    T _func;
    bool _dismiss = false;
};

template<typename T>
scope_guard<typename std::decay<T>::type> make_guard(T&& func)
{
    return scope_guard<typename std::decay<T>::type>(std::forward<T>(func));
}

}

#endif
