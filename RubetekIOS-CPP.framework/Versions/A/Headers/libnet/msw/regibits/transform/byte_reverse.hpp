#pragma once


namespace msw      {
namespace regibits {

    template <typename T>
    T byte_reverse(T val)
    {
        T ret = 0;
        for (int n = 0; n != sizeof(T); ++n)
        {
            ret <<= 8;
            ret |= (val & 0xff);
            val >>= 8;
        }
        return ret;
    }

}}
