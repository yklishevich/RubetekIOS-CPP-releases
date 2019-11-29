#pragma once

#include <type_traits>         // std::true_type, std::false_type

#include <msw/plain_types.hpp> // bit


namespace msw
{
    template <typename T>
    struct is_bit_plain
        : std::false_type
    {};
    template <>
    struct is_bit_plain<bit>
        : std::true_type
    {};
    template <>
    struct is_bit_plain<bit const>
        : std::true_type
    {};
}
