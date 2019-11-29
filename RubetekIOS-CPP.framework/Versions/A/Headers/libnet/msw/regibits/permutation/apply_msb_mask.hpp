#pragma once

#include <msw/regibits/location/msb_mask.hpp>


namespace msw      {
namespace regibits {

    template <typename T> 
    void apply_msb_mask(T& val, int count)
    {
        val &= msb_mask<T>(count);
    }

}}
