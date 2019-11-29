#pragma once

#include <string>


namespace msw       {
namespace file_path {

    inline std::string directory(std::string const& path)
    {
        size_t const pos = path.find_last_of("/\\");
        if (pos == std::string::npos) return "";
        return path.substr(0, pos);
    }
    inline std::string name(std::string const& path)
    {
        size_t const pos = path.find_last_of("/\\");
        if (pos == std::string::npos) return path;
        return path.substr(pos + 1);
    }
    inline std::string basic(std::string const& path)
    {
        std::string const nm = name(path);
        size_t const pos = nm.find_last_of('.');
        if (pos == std::string::npos) return nm;
        return nm.substr(0, pos);
    }
    inline std::string extension(std::string const& path)
    {
        size_t const pos = path.find_last_of('.');
        if (pos == std::string::npos) return "";
        if (path.find_first_of("/\\", pos) != std::string::npos) return "";
        return path.substr(pos + 1);
    }

}}
