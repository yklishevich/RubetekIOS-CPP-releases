#pragma once 

#include <msw/plain_types.hpp> // byte


namespace msw
{
    inline char nibble_to_char(byte v, bool uppercase = false)
    {
        return v + (v > 9 ? (uppercase ? 55 : 87) : 48);
    }
}
