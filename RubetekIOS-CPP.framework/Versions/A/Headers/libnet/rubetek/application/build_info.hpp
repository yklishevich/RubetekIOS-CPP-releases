#pragma once

#include <string>

#include <msw/format.hpp>
#include <msw/self/static_info.hpp>
#include <msw/version_of/included_libs.hpp>

#include <rubetek/version.hpp>


namespace rubetek
{
    inline std::string build_info()
    {
        return msw::format("build info: ", msw::self_static_info::mean(), " libs: ", msw::version_of_included_libs(), " rubetek-", version());
    }
}
