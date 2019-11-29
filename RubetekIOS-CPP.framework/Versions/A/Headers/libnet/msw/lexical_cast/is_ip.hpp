#pragma once 

#include <string>

#include <msw/std/regex.hpp>


namespace msw
{
    inline bool is_ip(std::string const& s)
    {
        msw::smatch what;
        static msw::regex expression("(\\d\\d?\\d?)\\.(\\d\\d?\\d?)\\.(\\d\\d?\\d?)\\.(\\d\\d?\\d?)");
        if (msw::regex_match(s, what, expression))
        {
            for (int n = 1; n != 5; ++n)
                if (std::stoi(what[n]) > 255) return false;
            return true;
        }
        return false;
    }
}
