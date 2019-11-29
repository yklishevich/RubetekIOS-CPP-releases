#pragma once 

#include <msw/zzz/lexical_cast/int_to/bin_rt.hpp>


namespace msw
{
    inline position<char> byte_to_bin(byte v, range<char> r)
    {
        return zzz::int_to_bin_rt(v, r);
    }
}
