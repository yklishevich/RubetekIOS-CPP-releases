#pragma once


namespace msw      {
namespace regibits {

    template <typename T>
    bool xor_bits(T val)
    {
        bool result = 0;
        int const size = sizeof(T) * 8;
        for (int n = 0; n < size; n++)
            result ^= ((val >> n) & 1);
        return result;
    }

}}
