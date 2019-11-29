#pragma once

#include <msw/regibits/location/lsb_mask.hpp>


namespace msw      {
namespace regibits {

    template <typename T> 
    void apply_lsb_mask(T& val, int count)
    {
        val &= lsb_mask<T>(count);
    }

}}
