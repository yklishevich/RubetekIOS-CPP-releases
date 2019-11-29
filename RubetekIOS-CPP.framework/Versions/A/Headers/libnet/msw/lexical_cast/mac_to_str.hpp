#pragma once 

#include <array>
#include <string>

#include <msw/make_range.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/int_to/fix_hex.hpp>


namespace msw
{
    inline void mac_to_str(range<byte const> mac, range<char> str)
    {
        if (mac.size() != size<byte>(6)) msw::throw_runtime_error("can't cast mac to str, size array of mac must be 6 bytes, actual size: ", mac.size());
        while (mac.has_items())
        {
            int_to_fix_hex(mac.pop_front(), str.pop_front(2));
            str.pop_front() = ':';
        }
    }
    inline std::string mac_to_str(range<byte const> mac)
    {
        std::array<char, 18> str;
        mac_to_str(mac, make_range<char>(str));
        str.back() = 0;
        return str.data();
    }
}
