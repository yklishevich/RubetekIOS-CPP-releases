#pragma once

#include <msw/config.hpp>
#include <msw/handle.hpp>
#include <msw/system_error/throw.hpp>
#ifdef MSW_WINAPI
#   include <msw/include/windows.hpp>
#else
#   include <signal.h>
#endif


namespace msw
{
    inline void terminate_process(handle::native_type h, bool& err)
    {
#ifdef MSW_WINAPI
        err = ::TerminateProcess(h, 0xffffffff) != TRUE;
#else
        err = ::kill(h, SIGKILL) != -1;
#endif
    }
    inline void terminate_process(handle::native_type h)
    {
        bool err;
        terminate_process(h, err);
        if (err) system_error::throw_exception("terminate process failed");
    }
#ifdef MSW_WINDOWS
    inline void terminate_process(int32 pid, bool& err)
    {
        msw::handle::native_type h = ::OpenProcess(PROCESS_TERMINATE, false, pid);
        if (h)
        {
            terminate_process(h, err);
            ::CloseHandle(h);
        }
        else err = true;
    }
#endif
}
