#pragma once

#include <msw/regibits/location/lsb_mask_rt.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    T lsb_mask(int index)
    {
        return lsb_mask_rt<T>(index);
    }

}}
