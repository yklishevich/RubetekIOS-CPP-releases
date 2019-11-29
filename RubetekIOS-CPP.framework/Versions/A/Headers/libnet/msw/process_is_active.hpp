#pragma once

#include <msw/config.hpp>
#include <msw/handle.hpp>
#include <msw/system_error/throw.hpp>
#ifdef MSW_WINAPI
#   include <msw/include/windows.hpp>
#else
#   include <sys/wait.h>
#endif


namespace msw
{
    inline bool process_is_active(handle::native_type h, bool& err)
    {
#ifdef MSW_WINAPI
        ::DWORD exit_code = 0;
        err = ::GetExitCodeProcess(h, &exit_code) != TRUE;
        return exit_code == STILL_ACTIVE;
#else
        /*
        // for child processes
        int status_ptr;
        int const ret = ::waitpid(h, &status_ptr, WNOHANG);
        err = ret == -1;
        return ret == 0;
        */
        int const ret = ::kill(h, 0);
        err = false;
        if (ret == 0) return true;
        if (errno == EPERM) return true;
        if (errno == ESRCH) return false;
        err = true;
        return false;
#endif
    }
    inline bool process_is_active(handle::native_type h)
    {
        bool err;
        bool const ret = process_is_active(h, err);
        if (err) system_error::throw_exception("call 'process is active' failed");
        return ret;
    }
}
