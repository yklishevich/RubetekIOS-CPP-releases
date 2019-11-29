#pragma once 

#include <msw/zzz/lexical_cast/int_to/fix_bin_rt.hpp>


namespace msw
{
    inline void byte_to_fix_bin_ilo(byte v, range<char> r)
    {
        zzz::int_to_fix_bin_rt(v, r, true);
    }
}
