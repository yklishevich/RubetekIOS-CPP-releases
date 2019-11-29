#pragma once


namespace msw      {
namespace regibits {

    template <typename T>
    T msb_bit()
    {
        return T(1) << (sizeof(T) * 8 - 1);
    }

}}
