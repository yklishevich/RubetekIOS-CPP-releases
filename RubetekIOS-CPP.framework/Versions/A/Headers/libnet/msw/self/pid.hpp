#pragma once 

#include <msw/config.hpp>

#ifdef MSW_WINAPI_SELF_PID
#   include <msw/include/windows.hpp>
#else
#   include <unistd.h>
#endif


namespace msw
{
    inline unsigned self_pid()
    {
        return
#ifdef MSW_WINAPI_SELF_PID
            ::GetCurrentProcessId();
#else
            ::getpid();
#endif
    }
}
