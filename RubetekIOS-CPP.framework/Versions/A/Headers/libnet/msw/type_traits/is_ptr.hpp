#pragma once

#include <type_traits>             // std::true_type, std::false_type

#include <msw/zzz/ptr/fwd.hpp>
#include <msw/zzz/bit_ptr/fwd.hpp>


namespace msw
{
    template <typename T>
    struct is_ptr
        : std::false_type
    {};
    template <typename T>
    struct is_ptr<ptr<T>>
        : std::true_type
    {};
    template <typename T>
    struct is_ptr<bit_ptr_t<T>>
        : std::true_type
    {};
}
