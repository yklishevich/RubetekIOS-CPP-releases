#pragma once


namespace msw      {
namespace regibits {

    template <typename T>
    T lsb_mask_rt(int index)
    {
        T v(0);
        for (; index; --index)
        {
            v <<= 1;
            v |= 1;
        }
        return v;
    }

}}
