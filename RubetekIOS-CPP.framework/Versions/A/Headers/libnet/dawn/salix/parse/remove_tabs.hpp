#pragma once

#include <string>


namespace dawn  {
namespace salix {

    inline std::string remove_tabs(std::string const& s, std::size_t& deep)
    {
        deep = s.find_first_not_of('\t');
        if (deep == std::string::npos) return "";
        return s.substr(deep);
    }

}}
