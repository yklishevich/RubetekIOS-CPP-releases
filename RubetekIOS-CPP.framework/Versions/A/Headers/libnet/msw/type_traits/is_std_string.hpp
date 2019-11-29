#pragma once

#include <string>
#include <type_traits> // std::true_type, std::false_type


namespace msw
{
    template <typename T>
    struct is_std_string
        : std::false_type
    {};
    template <>
    struct is_std_string<std::string>
        : std::true_type
    {};
}
