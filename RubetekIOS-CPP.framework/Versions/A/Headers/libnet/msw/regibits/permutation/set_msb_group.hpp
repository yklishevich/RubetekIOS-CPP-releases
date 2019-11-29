#pragma once

#include <msw/regibits/location/msb_mask.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    void set_msb_group(T& val, int count)
    {
        val |= msb_mask<T>(count);
    }

}}
