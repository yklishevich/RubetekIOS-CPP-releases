#pragma once

#include <msw/regibits/permutation/apply_lsb_mask.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    T select_lsb_group(T val, int count)
    {
        apply_lsb_mask<T>(val, count);
        return val;
    }

}}
