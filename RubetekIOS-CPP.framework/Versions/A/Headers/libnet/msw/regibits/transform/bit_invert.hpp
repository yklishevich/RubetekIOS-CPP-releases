#pragma once


namespace msw      {
namespace regibits {

    template <typename T>
    T bit_invert(T val)
    {
        return ~val;
    }

}}
