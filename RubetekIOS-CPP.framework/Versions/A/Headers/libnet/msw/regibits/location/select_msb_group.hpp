#pragma once

#include <msw/regibits/permutation/apply_msb_mask.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    T select_msb_group(T val, int count)
    {
        int const shift = sizeof(T) * 8 - count;
        return val >> shift;
    }

}}
