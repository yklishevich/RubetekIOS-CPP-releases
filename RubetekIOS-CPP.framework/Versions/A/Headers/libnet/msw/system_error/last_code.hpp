#pragma once

#include <msw/config.hpp>

#ifdef MSW_WINAPI
#   include <msw/include/windows.hpp>
#else
#   include <cerrno>
#endif


namespace msw          {
namespace system_error {

    inline int last_code()
    {
#ifdef MSW_WINAPI
        return ::GetLastError();
#else
        return errno;
#endif
    }

}}
