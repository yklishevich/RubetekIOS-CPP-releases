#pragma once 

#include <msw/range.hpp>
#include <msw/plain_types.hpp>                      // byte
#include <msw/algorithm/copy.hpp>
#include <msw/lexical_cast/byte_to/fix_hex_rt.hpp>
#include <msw/lexical_cast/byte_to/fix_hex_tbl.hpp>


namespace msw
{
    inline void byte_to_fix_hex(byte v, range<char> r)
    {
        //copy(byte_to_fix_hex_tbl(v), r);
        byte_to_fix_hex_rt(v, r);
    }
}
