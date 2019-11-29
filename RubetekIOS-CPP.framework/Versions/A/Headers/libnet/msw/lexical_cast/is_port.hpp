#pragma once 

#include <string>

#include <msw/std/regex.hpp>


namespace msw
{
    inline bool is_port(std::string const& s)
    {
        msw::smatch what;
        static msw::regex expression("(\\d\\d?\\d?\\d?\\d?)");
        if (msw::regex_match(s, what, expression))
            return std::stoi(what[1]) < 65536;
        return false;
    }
}
