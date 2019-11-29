#pragma once

#include <msw/zzz/entry_point/is_procedure.hpp>


namespace msw         {
namespace entry_point {
namespace zzz         {

    template <typename T, typename Fn, typename ...Args>
    typename std::enable_if
    <
        is_procedure<Fn(Args...)>::value
        , T
    >::type
    call_procedure_as_function(T default_value, Fn&& fn, Args&&... args)
    {
        fn(std::forward<Args>(args)...);
        return default_value;
    }

    template <typename T, typename Fn, typename ...Args>
    typename std::enable_if
    <
        !is_procedure<Fn(Args...)>::value
        , T
    >::type
    call_procedure_as_function(T, Fn&& fn, Args&&... args)
    {
        return fn(std::forward<Args>(args)...);
    }

}}}
