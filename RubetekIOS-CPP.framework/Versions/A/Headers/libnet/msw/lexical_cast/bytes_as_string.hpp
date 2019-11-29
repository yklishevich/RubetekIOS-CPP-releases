#pragma once 

#include <string>

#include <msw/buffer.hpp>
#include <msw/plain_types.hpp> // byte


namespace msw
{
    inline std::string bytes_as_string(range<char const> r)
    {
        return std::string(r.begin().to_pointer().native(), r.end().to_pointer().native());
    }
    inline std::string bytes_as_string(range<char> r)
    {
        return bytes_as_string(r.all<char const>());
    }

    inline std::string bytes_as_string(range<byte const> r)
    {
        return bytes_as_string(r.all<char>());
    }
    inline std::string bytes_as_string(range<byte> r)
    {
        return bytes_as_string(r.all<char>());
    }

    inline std::string bytes_as_string(buffer<char> const& buf)
    {
        return bytes_as_string(buf.all());
    }
    inline std::string bytes_as_string(buffer<byte> const& buf)
    {
        return bytes_as_string(buf.all<char>());
    }
}
