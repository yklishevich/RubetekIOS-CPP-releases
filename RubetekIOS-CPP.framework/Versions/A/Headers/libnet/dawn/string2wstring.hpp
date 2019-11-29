#pragma once

#include <locale>
#include <string>
#include <codecvt>


namespace dawn
{
    inline std::wstring string2wstring(std::string const& narrow_utf8_source_string)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wide = converter.from_bytes(narrow_utf8_source_string);
        return wide;
    }
    inline std::string wstring2string(std::wstring const& wide_utf16_source_string)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::string narrow = converter.to_bytes(wide_utf16_source_string);
        return narrow;
    }
}
