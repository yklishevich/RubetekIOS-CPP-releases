#pragma once

#include <array>
#include <string>
#include <cstring>                      // strerror

#include <msw/config.hpp>

#ifdef MSW_WINAPI
#   include <msw/include/windows.hpp>
#else
#   include <cerrno>
#endif


namespace msw          {
namespace system_error {

    inline std::string description(int error_code)
    {
#ifdef MSW_WINAPI
        std::array<char, 256> s;
        int result = ::FormatMessageA
        (
            FORMAT_MESSAGE_FROM_SYSTEM
            , 0
            , error_code
            , MAKELANGID(LANG_ENGLISH/*LANG_NEUTRAL*/, SUBLANG_DEFAULT)
            , s.data()
            , DWORD(s.size() - 1)
            , 0
        );
        if (result && (result < static_cast<int>(s.size())))
        {
            s.back() = 0;
            char* pch = strchr(s.data(),'\r');
            if (pch) *pch = 0;
            return s.data();
        }
        return "";
#else
#   ifdef MSW_MSVC
        std::array<char, 256> s;
        ::strerror_s(s.data(), s.size(), error_code);
        return s.data();
#   else
        return ::strerror(error_code);
#   endif
#endif
    }

}}
