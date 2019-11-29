#pragma once

#include <string>


#define RUBETEK_VERSION 50


namespace rubetek
{
    inline std::string const& version()
    {
        static std::string const v = std::to_string(RUBETEK_VERSION);
        return v;
    }
}
