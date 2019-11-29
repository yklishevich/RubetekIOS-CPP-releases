#pragma once 

#include <string>

#include <msw/zzz/lexical_cast/int_from/dec.hpp>


namespace msw
{
    template <typename T>
    T int_from_dec(std::string const& s)
    {
        return zzz::int_from_dec_impl<T>()(s);
    }
}
