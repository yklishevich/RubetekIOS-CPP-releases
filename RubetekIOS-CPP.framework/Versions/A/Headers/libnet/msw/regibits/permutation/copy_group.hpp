#pragma once

#include <msw/regibits/location/select_group.hpp>
#include <msw/regibits/permutation/reset_group.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    void copy_group_n(T src, int s_fst, T& dst, int d_fst, int count)
    {
        reset_group_n<T>(dst, d_fst, count);
        dst |= (select_group_n<T>(src, s_fst, count) << d_fst);
    }
    template <typename T>
    void copy_group(T src, int s_fst, int s_lst, T& dst, int d_fst)
    {
        copy_group_n<T>(src, s_fst, dst, d_fst, s_lst - s_fst);
    }

}}
