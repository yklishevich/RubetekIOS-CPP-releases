#pragma once 

#include <string>

#include <boost/filesystem.hpp>

#include <msw/config.hpp>
#include <msw/self/exe_name.hpp>


namespace msw
{
    inline std::string self_path()
    {
        return boost::filesystem::path(self_exe_name()).remove_filename().string();
    }
}
