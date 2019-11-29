#pragma once


namespace msw      {
namespace regibits {

    template <typename T>
    T bit_mask(int pos)
    {
        T mask = 1;
        mask <<= pos;
        return mask;
    }

}}
