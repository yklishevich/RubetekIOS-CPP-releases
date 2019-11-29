#pragma once

#include <msw/regibits/permutation/apply_lsb_mask.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    T select_group_n(T val, int fst, int count)
    {
        val >>= fst;
        apply_lsb_mask<T>(val, count);
        return val;
    }
    template <typename T>
    T select_group(T val, int fst, int lst)
    {
        return select_group_n<T>(val, fst, lst - fst);
    }

}}
