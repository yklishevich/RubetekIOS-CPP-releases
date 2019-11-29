#pragma once

#include <type_traits>                                 // std::enable_if

#include <msw/int.hpp>
#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/lexical_cast/byte_to/hex.hpp>
#include <msw/lexical_cast/byte_to/nibble_to_char.hpp>
#include <msw/lexical_cast/int_to/hex_size.hpp>
#include <msw/regibits/permutation/apply_msb_mask.hpp>


namespace msw
{
    template <typename T>
    typename std::enable_if<sizeof(T) != 1, position<char>>::type
    int_to_hex(T v, range<char> r)
    {
        uint p = int_to_hex_size<T>().count();
        T vx = v;
        for (;;)
        {
            T nibble = vx;
            regibits::apply_msb_mask(nibble, 4);
            if (nibble || (p == 1)) break;
            vx <<= 4;
            --p;
        }
        position<char> const end = r.begin(p);
        for (; p; v >>= 4)
            r[--p] = nibble_to_char(v & 0x0f);
        return end;
    }
    template <typename T>
    typename std::enable_if<sizeof(T) == 1, position<char>>::type
    int_to_hex(T v, range<char> r)
    {
        return byte_to_hex(v, r);
    }

    template <typename T>
    buffer<char> int_to_hex(T v)
    {
        buffer<char> buf(int_to_hex_size<T>() + 1);
        buf.erase_from_to_back(int_to_hex(v, buf.all()));
        buf.push_back_silent(char(0));
        return buf;
    }
}
