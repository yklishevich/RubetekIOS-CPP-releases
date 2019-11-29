#pragma once

#include <string>
#include <utility>

#include <msw/throw_runtime_error.hpp>


namespace dawn
{
    inline std::pair<std::string, std::string> split_string(std::string const& s, char delimiter)
    {
        size_t pos = s.find_first_of(delimiter);
        if (pos == std::string::npos || (pos == (s.size() - 1))) msw::throw_runtime_error("there is no delimiter: ", s);
        return std::make_pair(s.substr(0, pos), s.substr(pos + 1));
    }
}
