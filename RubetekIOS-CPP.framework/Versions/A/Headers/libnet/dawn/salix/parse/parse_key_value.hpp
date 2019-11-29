#pragma once

#include <string>
#include <utility>


namespace dawn  {
namespace salix {

    inline std::pair<std::string, std::string> parse_key_value(std::string s)
    {
        auto pos = s.find_first_of("\t ");
        if (pos == std::string::npos) return {s, ""};
        MSW_ASSERT(pos);
        std::string const key = s.substr(0, pos);
        s = s.substr(pos + 1);
        pos = s.find_first_not_of("\t ");
        MSW_ASSERT(pos != std::string::npos);
        MSW_ASSERT(s.size() > pos);
        std::string const value = s.substr(pos);
        return {key, value};
    }

}}
