#pragma once

#include <msw/regibits/location/lsb_mask.hpp>
#include <msw/regibits/location/msb_mask.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    T interval_mask(int fst, int lst)
    {
        return lsb_mask<T>(lst) & msb_mask<T>(sizeof(T) * 8 - fst);
    }
    template <typename T>
    T interval_mask_n(int fst, int count)
    {
        return interval_mask<T>(fst, fst + count);
    }

}}
