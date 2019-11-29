#pragma once 

#include <type_traits>                                  // std::enable_if

#include <msw/int.hpp>
#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/lexical_cast/byte_to/fix_hex.hpp>
#include <msw/lexical_cast/byte_to/nibble_to_char.hpp>
#include <msw/lexical_cast/int_to/hex_size.hpp>


namespace msw
{
    template <typename T>
    typename std::enable_if<sizeof(T) != 1>::type
    int_to_fix_hex(T v, range<char> r)
    {
        uint p = int_to_hex_size<T>().count();
        for (; p; v >>= 4)
            r[--p] = nibble_to_char(v & 0x0f);
    }
    template <typename T>
    typename std::enable_if<sizeof(T) == 1>::type
    int_to_fix_hex(T v, range<char> r)
    {
        byte_to_fix_hex(v, r);
    }

    template <typename T>
    buffer<char> int_to_fix_hex(T v)
    {
        buffer<char> buf(int_to_hex_size<T>() + 1);
        int_to_fix_hex(v, buf.all());
        buf.back() = 0;
        return buf;
    }
}
