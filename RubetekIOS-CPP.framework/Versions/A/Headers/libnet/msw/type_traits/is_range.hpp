#pragma once

#include <type_traits>           // std::true_type, std::false_type

#include <msw/zzz/range/fwd.hpp>


namespace msw
{
    template <typename T>
    struct is_range
        : std::false_type
    {};
    template <typename T>
    struct is_range<range<T>>
        : std::true_type
    {};
}
