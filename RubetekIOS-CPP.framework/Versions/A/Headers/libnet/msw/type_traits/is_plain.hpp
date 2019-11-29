#pragma once

#include <type_traits>         // std::conditional, std::is_pod, std::is_pointer
#include <msw/plain_types.hpp> // bit


namespace msw
{
    template <typename T>
    struct is_plain
        : std::conditional
        <
              std::is_pod<T>::value && !std::is_pointer<T>::value
            , std::true_type
            , std::false_type
        >::type
    {};
    template <>
    struct is_plain<bit>
        : std::true_type
    {};
}
