#pragma once

#include <msw/regibits/location/bit_mask.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    void reset_bit(T& val, int pos)
    {
        val &= ~bit_mask<T>(pos);
    }

}}
