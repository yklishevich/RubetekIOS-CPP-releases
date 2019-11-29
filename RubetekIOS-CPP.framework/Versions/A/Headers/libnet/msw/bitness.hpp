#pragma once

#include <cstddef>     // std::size_t
#include <type_traits> // std::conditional, std::true_type, std::false_type


namespace msw
{
    typedef std::conditional
    <
        sizeof(std::size_t) == 4
        , std::true_type, std::false_type
    >::type bitness_32;

    typedef std::conditional
    <
        sizeof(std::size_t) == 8
        , std::true_type, std::false_type
    >::type bitness_64;
}
