#pragma once

#include <msw/regibits/location/msb_bit.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    T msb_mask_rt(int index)
    {
        T const one = msb_bit<T>();
        T v(0);
        for (; index; --index)
        {
            v >>= 1;
            v |= one;
        }
        return v;
    }

}}
