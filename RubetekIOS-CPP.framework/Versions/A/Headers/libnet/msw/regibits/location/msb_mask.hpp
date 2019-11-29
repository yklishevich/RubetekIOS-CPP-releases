#pragma once

#include <msw/regibits/location/msb_mask_rt.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    T msb_mask(int index)
    {
        return msb_mask_rt<T>(index);
    }

}}
