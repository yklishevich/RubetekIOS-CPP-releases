#pragma once

#include <string>

#include <msw/config.hpp>

#ifdef MSW_WINDOWS
#   include <msw/console/win.hpp>
#else
#   include <msw/console/lin.hpp>
#endif


namespace msw     {
namespace console {

    inline void set_title(std::string const& title)
    {
#ifdef MSW_WINDOWS
        win::set_title(title);
#else
        lin::set_title(title);
#endif
    }

}}
