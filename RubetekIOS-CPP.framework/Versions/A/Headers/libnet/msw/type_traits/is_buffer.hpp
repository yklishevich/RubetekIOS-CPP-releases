#pragma once

#include <type_traits>            // std::true_type, std::false_type

#include <msw/zzz/buffer/fwd.hpp>


namespace msw
{
    template <typename T>
    struct is_buffer
        : std::false_type
    {};
    template <typename T>
    struct is_buffer<buffer<T>>
        : std::true_type
    {};
}
