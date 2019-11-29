#pragma once 

#include <string>

#include <msw/zzz/lexical_cast/format_count.hpp>


namespace msw
{
    template <typename ValueType>
    std::string format_count(ValueType value, std::string const& measure_unit = "", bool fixed_float = false, int precision = 2)
    {
        return zzz::format_count<ValueType>()(value, fixed_float, precision, measure_unit);
    }
}
