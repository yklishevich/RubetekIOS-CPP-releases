#pragma once

#include <array>
#include <type_traits> // std::true_type, std::false_type


namespace msw
{
    template <typename T>
    struct is_std_array
        : std::false_type
    {};
    template <typename T, std::size_t N>
    struct is_std_array<std::array<T, N>>
        : std::true_type
    {};
}
