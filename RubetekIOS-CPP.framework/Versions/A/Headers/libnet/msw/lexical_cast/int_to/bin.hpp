#pragma once 

#include <msw/lexical_cast/byte_to/bin.hpp>
#include <msw/zzz/lexical_cast/int_to/bin_rt.hpp>


namespace msw
{
    template <typename T>
    typename std::enable_if<sizeof(T) != 1, position<char>>::type
    int_to_bin(T v, range<char> r)
    {
        return zzz::int_to_bin_rt(v, r);
    }
    template <typename T>
    typename std::enable_if<sizeof(T) == 1, position<char>>::type
    int_to_bin(T v, range<char> r)
    {
        return byte_to_bin(v, r);
    }

    template <typename T>
    buffer<char> int_to_bin(T v)
    {
        buffer<char> buf(int_to_bin_size<T>() + 1);
        buf.erase_from_to_back(int_to_bin(v, buf.all()));
        buf.push_back_silent(char(0));
        return buf;
    }
}
