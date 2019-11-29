#pragma once

#include <string>
#include <locale>
#include <vector>

#include <msw/config.hpp>

#ifdef MSW_GCC
#else
#   include <codecvt>
#endif


namespace dawn
{
    inline std::string utf8_to_string(char const* utf8str, std::locale const& loc = std::locale(".1251"))
    {
#ifdef MSW_GCC
        return "";
#else
        std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
        std::wstring wstr = wconv.from_bytes(utf8str);
        std::vector<char> buf(wstr.size());
        if (!buf.empty()) std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
        return std::string(buf.data(), buf.size());
#endif
    }
    inline std::string utf8_to_string(std::string const& utf8str, std::locale const& loc = std::locale(".1251"))
    {
        return utf8_to_string(utf8str.c_str(), loc);
    }
}
