#pragma once 

#include <string>

#include <msw/std/regex.hpp>


namespace msw
{
    template <typename Target, typename Source>
    void can_cast(Source const& s)
    {
        Target result;
        return boost::conversion::try_lexical_convert(s, result);
    }
}
