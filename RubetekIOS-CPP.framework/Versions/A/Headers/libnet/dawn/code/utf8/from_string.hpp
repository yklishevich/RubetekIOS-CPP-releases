#pragma once

#include <string>
#include <locale>
#include <vector>

#include <msw/config.hpp>

#ifndef MSW_MINGW
#   include <codecvt>
#endif


namespace dawn
{
    /*
    inline std::string utf8_from_1251(char const* utf8str, std::locale const& loc = std::locale(".1251"))
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
        std::wstring wstr = wconv.from_bytes(utf8str);
        std::vector<char> buf(wstr.size());
        std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
        return std::string(buf.data(), buf.size());
    }
    inline std::string utf8_from_1251(std::string const& utf8str, std::locale const& loc = std::locale(".1251"))
    {
        return utf8_from_1251(utf8str.c_str(), loc);
    }
    */
}
