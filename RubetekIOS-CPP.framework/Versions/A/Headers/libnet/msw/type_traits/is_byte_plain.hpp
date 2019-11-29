#pragma once

#include <type_traits> // std::conditional, std::is_pod, std::is_pointer, std::true_type, std::false_type


namespace msw
{
    template <typename T>
    struct is_byte_plain
        : std::conditional
        <
              std::is_pod<T>::value && !std::is_pointer<T>::value
            , std::true_type
            , std::false_type
        >::type
    {};
}
