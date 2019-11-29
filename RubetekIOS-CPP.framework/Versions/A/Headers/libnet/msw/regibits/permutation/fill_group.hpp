#pragma once

#include <msw/regibits/permutation/set_group.hpp>
#include <msw/regibits/permutation/reset_group.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    void fill_group(T& val, int fst, int lst, bool x)
    {
        if (x) set_group(val, fst, lst);
        else reset_group(val, fst, lst);
    }
    template <typename T>
    void fill_group_n(T& val, int fst, int count, bool x)
    {
        fill_group<T>(val, fst, fst + count, x);
    }

}}
