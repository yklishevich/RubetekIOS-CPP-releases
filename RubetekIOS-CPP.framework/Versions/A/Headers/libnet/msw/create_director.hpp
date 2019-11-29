#pragma once

#include <string>

#include <boost/filesystem.hpp>

#include <msw/file_path.hpp>


namespace msw
{
    inline void create_directory(std::string const& dir)
    {
        if (!boost::filesystem::exists(dir)) boost::filesystem::create_directories(dir);
    }
    inline void create_directory_for_file(std::string const& file)
    {
        std::string const dir = file_path::directory(file);
        if (!dir.empty()) create_directory(dir);
    }
}
