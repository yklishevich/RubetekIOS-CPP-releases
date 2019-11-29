#pragma once


namespace msw      {
namespace regibits {

    template <typename T>
    T bit_reverse(T val)
    {
        T res(0);
        for (int i = 0; i < sizeof(T) * 8; ++i)
        {
            res <<= 1;
            res |= ((val >> i) & 1);
        }
        return res;
    }

}}
