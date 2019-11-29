#pragma once 

#include <msw/range.hpp>
#include <msw/plain_types.hpp>                         // byte
#include <msw/lexical_cast/byte_to/fix_hex_rt.hpp>
#include <msw/lexical_cast/byte_to/nibble_to_char.hpp>


namespace msw
{
    inline position<char> byte_to_hex_rt(byte v, range<char> r)
    {
        if (v >> 4)
        {
            byte_to_fix_hex_rt(v, r);
            return r.begin(2);
        }
        r[0] = nibble_to_char(v & 0x0f);
        return r.begin(1);
    }
}
