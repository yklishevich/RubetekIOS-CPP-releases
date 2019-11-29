#pragma once

#include <type_traits>                                 // std::make_unsigned

#include <msw/range.hpp>
#include <msw/zzz/lexical_cast/int_to/bin_impl_rt.hpp>


namespace msw {
namespace zzz {

    template <typename T>
    void int_to_bin_rt(T v, uint count, range<char> r, bool intel_like_order = false, char zero = '-', char one = '*')
    {
        typedef typename std::make_unsigned<T>::type U;
        return int_to_bin_impl_rt(static_cast<U>(v), count, r, intel_like_order, zero, one);
    }

    template <typename T>
    position<char> int_to_bin_rt(T v, range<char> r, bool intel_like_order = false, char zero = '-', char one = '*')
    {
        typedef typename std::make_unsigned<T>::type U;
        return int_to_bin_impl_rt(static_cast<U>(v), r, intel_like_order, zero, one);
    }

}}
