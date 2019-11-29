#pragma once

#include <msw/int.hpp>
#include <msw/range.hpp>
#include <msw/regibits/location/msb_bit.hpp>
#include <msw/lexical_cast/int_to/bin_size.hpp>
#include <msw/lexical_cast/byte_to/bit_to_char.hpp>


namespace msw {
namespace zzz {

    template <typename T>
    void int_to_bin_impl_rt(T v, uint count, range<char> r, bool intel_like_order, char zero, char one)
    {
        static_assert(std::is_unsigned<T>::value, "T must be unsigned type");

        for (uint pos = 0; pos != count; ++pos)
        {
            r[intel_like_order ? count - pos - 1 : pos] = bit_to_char(v & 1, zero, one);
            v >>= 1;
        }
    }

    template <typename T>
    position<char> int_to_bin_impl_rt(T v, range<char> r, bool intel_like_order = false, char zero = '-', char one = '*')
    {
        static_assert(std::is_unsigned<T>::value, "T must be unsigned type");

        uint count = int_to_bin_size<T>().count();
        if (intel_like_order)
        {
            T vx = v;
            for (;;)
            {
                if (vx & regibits::msb_bit<T>()) break;
                if (--count == 1) break;
                vx <<= 1;
            }
        }
        else
        {
            for (;;)
            {
                if (v & 1) break;
                v >>= 1;
                if (--count == 1) break;
            }
        }
        int_to_bin_impl_rt(v, count, r, intel_like_order, zero, one);
        return r.begin(count);
    }

}}
