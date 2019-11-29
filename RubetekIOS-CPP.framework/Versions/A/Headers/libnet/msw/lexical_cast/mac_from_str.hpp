#pragma once 

#include <string>

#include <msw/make_range.hpp>
#include <msw/lexical_cast/int_from/hex.hpp>


namespace msw
{
    inline void mac_from_str(range<char const> str, range<byte> mac)
    {
        if (str.size() < size<char>(6 * 3 - 1)) throw std::runtime_error("can't cast mac from str (small string)");
        for (int i = 0;; ++i)
        {
            mac.pop_front() = int_from_hex<byte>(str.pop_front(2));
            if (i == 5) break;
            str.pop_front();
        }
    }
    inline void mac_from_str(std::string const& str, range<byte> mac)
    {
        mac_from_str(make_range(str), mac);
    }
}
