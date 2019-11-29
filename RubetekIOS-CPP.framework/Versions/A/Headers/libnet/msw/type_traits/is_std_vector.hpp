#pragma once

#include <vector>
#include <type_traits> // std::true_type, std::false_type


namespace msw
{
    template <typename T>
    struct is_std_vector
        : std::false_type
    {};
    template <typename T>
    struct is_std_vector<std::vector<T>>
        : std::true_type
    {};
}
