#pragma once 

#include <msw/range.hpp>
#include <msw/plain_types.hpp>                  // byte
#include <msw/algorithm/copy.hpp>
#include <msw/lexical_cast/byte_to/hex_rt.hpp>
#include <msw/lexical_cast/byte_to/hex_tbl.hpp>


namespace msw
{
    inline position<char> byte_to_hex(byte v, range<char> r)
    {
        //copy(byte_to_hex_tbl(v), r);
        return byte_to_hex_rt(v, r);
    }
}
