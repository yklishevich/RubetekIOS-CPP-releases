#pragma once

#include <msw/buffer.hpp>
#include <msw/lexical_cast/int_to/hex.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>


namespace msw
{
    template <typename T>
    std::string int_to_hex_std_string(T v)
    {
        auto str = int_to_hex(v);
        auto r = str.all();
        r.pop_back();
        return bytes_as_string(r);
    }
}
