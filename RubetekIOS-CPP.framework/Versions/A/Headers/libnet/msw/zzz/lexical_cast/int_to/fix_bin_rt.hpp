#pragma once

#include <msw/range.hpp>
#include <msw/zzz/lexical_cast/int_to/bin_rt.hpp>


namespace msw {
namespace zzz {

    template <typename T>
    void int_to_fix_bin_rt(T v, range<char> r, bool intel_like_order = false, char zero = '-', char one = '*')
    {
        int_to_bin_rt(v, int_to_bin_size<T>().count(), r, intel_like_order, zero, one);
    }

}}
