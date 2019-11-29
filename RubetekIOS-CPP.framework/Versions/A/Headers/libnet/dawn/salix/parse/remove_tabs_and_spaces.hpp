#pragma once

#include <string>


namespace dawn  {
namespace salix {

    inline std::string remove_tabs_and_spaces(std::string const& s)
    {
        std::size_t pos = s.find_first_not_of("\t ");
        if (pos == std::string::npos) return "";
        return s.substr(pos);
    }

}}
