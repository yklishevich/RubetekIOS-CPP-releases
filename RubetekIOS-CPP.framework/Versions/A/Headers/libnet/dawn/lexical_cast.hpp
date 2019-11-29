#pragma once

#include <string>

#include <boost/lexical_cast.hpp>

#include <msw/lexical_cast/bool_from_string.hpp>


namespace dawn
{
    template<typename Target, typename Source>
    Target lexical_cast(Source const& src)
    {
        return boost::lexical_cast<Target>(src);
    }
    template<>
    bool lexical_cast<bool, std::string>(std::string const& src)
    {
        return msw::bool_from_string(src);
    }
}
