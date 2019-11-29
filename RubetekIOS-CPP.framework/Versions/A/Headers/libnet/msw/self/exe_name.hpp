#pragma once 

#include <array>
#include <string>

#include <msw/config.hpp>
#include <msw/system_error/throw.hpp>

#ifdef MSW_APPLE
#   include <mach-o/dyld.h>
#else
#   ifdef MSW_WINAPI_SELF_EXE_NAME
#       include <msw/include/windows.hpp>
#   else
#       include <unistd.h>
#   endif
#endif


namespace msw
{
    inline std::string self_exe_name(bool& error)
    {
        std::array<char, 1024> filename;
#ifdef MSW_APPLE
        uint32_t len = static_cast<uint32_t>(filename.size());
#endif
        int ret =
#ifdef MSW_APPLE
            ::_NSGetExecutablePath(filename.data(), &len);
        error = ret != 0;
        //if (!error) filename[len] = 0;
#else
#   ifdef MSW_WINAPI_SELF_EXE_NAME
            ::GetModuleFileNameA(0
#   else
            ::readlink("/proc/self/exe"
#   endif
            , filename.data(), static_cast<unsigned>(filename.size()));
        error = ret ==
#   ifdef MSW_WINAPI_SELF_EXE_NAME
            0;
#   else
            -1;
        if (!error) filename[ret] = 0;
#   endif
#endif
        return error ? "" : filename.data();
    }

    inline std::string self_exe_name()
    {
        bool error;
        std::string result = self_exe_name(error);
        if (error) system_error::throw_exception("can't get self exe name");
        return std::move(result);
    }
}
