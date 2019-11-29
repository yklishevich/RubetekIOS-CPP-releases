#pragma once

#include <string>

#include <boost/filesystem.hpp>


namespace msw
{
    inline std::string initial_path()
    {
        return boost::filesystem::initial_path().string();
    }
}
