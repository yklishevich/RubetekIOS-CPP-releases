#pragma once

#include <string>

#include <msw/config.hpp>
#include <msw/include/windows.hpp>
#include <msw/self/path.hpp>
#include <msw/system_error/throw.hpp>


namespace msw
{
    inline void set_current_dir(std::string const& dir = self_path())
    {
        if (!::SetCurrentDirectoryA(dir.c_str())) system_error::throw_exception("can't set current directory");
    }
}
