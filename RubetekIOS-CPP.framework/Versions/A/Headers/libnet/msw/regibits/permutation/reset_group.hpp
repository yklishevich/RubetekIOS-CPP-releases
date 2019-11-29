#pragma once

#include <msw/regibits/location/interval_mask.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    void reset_group(T& val, int fst, int lst)
    {
        val &= ~interval_mask<T>(fst, lst);
    }
    template <typename T>
    void reset_group_n(T& val, int fst, int count)
    {
        reset_group<T>(val, fst, fst + count);
    }

}}
