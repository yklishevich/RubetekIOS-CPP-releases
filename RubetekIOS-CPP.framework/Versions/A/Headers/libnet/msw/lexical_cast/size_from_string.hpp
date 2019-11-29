#pragma once 

#include <string>

#include <msw/int.hpp>
#include <msw/zzz/lexical_cast/count_from_string.hpp>


namespace msw
{
    template <typename T>
    T size_from_string(std::string const& s)
    {
        return zzz::count_from_string<T, 1024>(s);
    }
    inline uint size_from_string(std::string const& s)
    {
        return size_from_string<uint>(s);
    }
}
