#pragma once

#include <type_traits>          // std::true_type, std::false_type

#include <msw/zzz/size/fwd.hpp>


namespace msw
{
    template <typename T>
    struct is_size
        : std::false_type
    {};
    template <typename T>
    struct is_size<size<T>>
        : std::true_type
    {};
}
