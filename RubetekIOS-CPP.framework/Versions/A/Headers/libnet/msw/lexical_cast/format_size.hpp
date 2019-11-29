#pragma once 

#include <string>

#include <msw/size.hpp>
#include <msw/plain_types.hpp>                   // byte
#include <msw/zzz/lexical_cast/format_count.hpp>


namespace msw
{
    inline std::string format_size(size<byte> value, std::string const& measure_unit = "", bool fixed_float = false, int precision = 2)
    {
        return zzz::format_count<size<byte>::count_type, 1024>()(value.count(), fixed_float, precision, measure_unit);
    }
    template <typename T>
    std::string format_size(T value, std::string const& measure_unit = "", bool fixed_float = false, int precision = 2)
    {
        return zzz::format_count<T, 1024>()(value, fixed_float, precision, measure_unit);
    }
}
