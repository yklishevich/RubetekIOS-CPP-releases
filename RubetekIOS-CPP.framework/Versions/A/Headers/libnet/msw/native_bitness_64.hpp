#pragma once 

#include <msw/assert.hpp>
#include <msw/system_error/throw.hpp>

#ifdef MSW_WINDOWS
#   include <msw/include/windows.hpp>
#endif


namespace msw
{
    inline bool native_bitness_64()
    {
#ifdef MSW_BITNESS_32
#   ifdef MSW_WINDOWS
        typedef BOOL(WINAPI*fn_IsWow64Process)(HANDLE, PBOOL);
        fn_IsWow64Process IsWow64Process = (fn_IsWow64Process)::GetProcAddress(::GetModuleHandle("kernel32"), "IsWow64Process");
        bool is_wow64 = false;
        if (IsWow64Process)
        {
            int status;
            if (!IsWow64Process(::GetCurrentProcess(), &status))
            {
                system_error::throw_exception("call IsWow64Process fail");
            }
            is_wow64 = status != 0;
        }
        return is_wow64;
#   else
        /*
        uname -m
            i686
            x86_64
        can be implemented by redirecting of console.... but ios, android...
        */
        return false;
#   endif
#else
        return true;
#endif
    }
}
