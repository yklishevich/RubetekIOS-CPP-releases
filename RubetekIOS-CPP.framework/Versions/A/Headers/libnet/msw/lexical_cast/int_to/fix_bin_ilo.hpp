#pragma once 

#include <msw/lexical_cast/byte_to/fix_bin_ilo.hpp>
#include <msw/zzz/lexical_cast/int_to/fix_bin_rt.hpp>


namespace msw 
{
    template <typename T>
    typename std::enable_if<sizeof(T) != 1>::type
    int_to_fix_bin_ilo(T v, range<char> r)
    {
        zzz::int_to_fix_bin_rt(v, r, true);
    }
    template <typename T>
    typename std::enable_if<sizeof(T) == 1>::type
    int_to_fix_bin_ilo(T v, range<char> r)
    {
        byte_to_fix_bin_ilo(v, r);
    }

    template <typename T>
    buffer<char> int_to_fix_bin_ilo(T v)
    {
        buffer<char> buf(int_to_bin_size<T>() + 1);
        int_to_fix_bin_ilo(v, buf.all());
        buf.back() = 0;
        return buf;
    }
}
