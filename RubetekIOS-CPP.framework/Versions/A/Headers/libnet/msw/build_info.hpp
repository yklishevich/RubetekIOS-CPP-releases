#pragma once

#include <map>
#include <string>

#include <msw/config.hpp>
#include <msw/self/static_info.hpp>
#include <msw/version_of/included_libs.hpp>


namespace msw
{
    inline std::string build_info_laconic
    (
          std::string version
        , std::string build = std::string()
    )
    {
        std::string info = "v-" + version;
        if (!build.empty()) info += " build-" + build;
        return info;
    }
    inline std::string build_info_quick
    (
          std::string version
        , std::string build = std::string()
    )
    {
        std::string info = build_info_laconic(version, build);
        info += " " + self_static_info::quick();
        return info;
    }
    inline std::string build_info
    (
          std::string                        version
        , std::string                        build     = std::string()
        , std::map<std::string, std::string> libraries = std::map<std::string, std::string>()
    )
    {
        std::string info = build_info_laconic(version, build);
        info += " " + self_static_info::total();
        info += " (libs: " + version_of_included_libs();
        if (!libraries.empty())
        {
            for (auto const& lib : libraries)
                info += " " + lib.first + "-" + lib.second;
        }
        info += ")";
        return info;
    }
    inline std::string build_info
    (
          std::string                        version
        , std::map<std::string, std::string> libraries
    )
    {
        return build_info(version, "", libraries);
    }
}
