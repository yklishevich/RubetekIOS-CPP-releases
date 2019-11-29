#pragma once 

#include <msw/range.hpp>
#include <msw/plain_types.hpp>                         // byte
#include <msw/lexical_cast/byte_to/nibble_to_char.hpp>


namespace msw
{
    inline void byte_to_fix_hex_rt(byte v, range<char> r)
    {
        r[0] = nibble_to_char(v >> 4);
        r[1] = nibble_to_char(v & 0x0f);
    }
}
