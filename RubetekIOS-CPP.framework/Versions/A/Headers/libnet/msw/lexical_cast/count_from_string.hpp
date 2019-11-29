#pragma once 

#include <string>

#include <msw/int.hpp>
#include <msw/zzz/lexical_cast/count_from_string.hpp>


namespace msw
{
    template <typename T>
    T count_from_string(std::string const& s)
    {
        return zzz::count_from_string<T>(s);
    }
    inline uint count_from_string(std::string const& s)
    {
        return count_from_string<uint>(s);
    }
}
