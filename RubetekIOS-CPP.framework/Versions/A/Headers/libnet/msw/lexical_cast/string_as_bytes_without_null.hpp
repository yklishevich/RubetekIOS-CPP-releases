
#pragma once 

#include <string>

#include <msw/make_range.hpp>
#include <msw/make_range_native.hpp>


namespace msw 
{
    inline range<byte const> string_as_bytes_without_null(char const* s)
    {
        return make_range_native(s, ::strlen(s)).all<byte>();
    }
    inline range<byte const> string_as_bytes_without_null(std::string const& s)
    {
        auto result = make_range<byte>(s);
        result.pop_back();
        return result;
    }
}
