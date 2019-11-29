#pragma once


namespace msw      {
namespace regibits {

    template <typename T>
    bool select_lsb_bit(T val, int pos)
    {
        val >>= pos;
        return val & 1;
    }
    template <typename T>
    bool select_msb_bit(T val, int pos)
    {
        int const shift = sizeof(T) * 8 - 1 - pos;
        val >>= shift;
        return val & 1;
    }

}}
