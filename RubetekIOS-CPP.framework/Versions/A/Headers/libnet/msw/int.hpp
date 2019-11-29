#pragma once

#include <type_traits>         // std::conditional

#include <msw/bitness.hpp>
#include <msw/plain_types.hpp> // int32, int64, uint32, uint64


namespace msw
{
    typedef std::conditional
    <
        bitness_32::value
        , int32, int64
    >::type                  sint;
    typedef std::conditional
    <
        bitness_32::value
        , uint32, uint64
    >::type                  uint;

    static uint64 const uint_max = static_cast<uint>(-1);
}
