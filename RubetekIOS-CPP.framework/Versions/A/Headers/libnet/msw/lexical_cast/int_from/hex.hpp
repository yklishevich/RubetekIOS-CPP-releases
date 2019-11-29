#pragma once 

#include <string>

#include <msw/range.hpp>
#include <msw/lexical_cast/byte_from/nibble_from_char.hpp>


namespace msw
{
    template <typename T>
    T int_from_hex(range<char const> r)
    {
        if (r.size().count() > (sizeof(T) * 2)) throw std::runtime_error("string size exceeds the allowable value");
        T v = 0;
        int n = 0;
        while (r.has_items())
        {
            T nibble = nibble_from_char(r.pop_back());
            v |= (nibble << (4 * n++));
        }
        return v;
    }
    inline int int_from_hex(range<char const> r)
    {
        return int_from_hex<int>(r);
    }

    template <typename T>
    T int_from_hex(std::string const& s)
    {
        return int_from_hex<T>(msw::make_range(s));
    }
    inline int int_from_hex(std::string const& s)
    {
        return int_from_hex<int>(s);
    }
}
