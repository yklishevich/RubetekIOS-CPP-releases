#pragma once

#include <string>
#include <vector>

#include <msw/buffer.hpp>
#ifdef MSW_MSVC
#   include <msw/include/windows.hpp>
#endif


namespace dawn
{
    inline msw::buffer<char> utf8_from_w1251(msw::range<char const> w1251_str, bool push_back_null = false)
    {
#ifdef MSW_MSVC
        if (w1251_str.has_items())
        {
            int len = MultiByteToWideChar(1251, 0, w1251_str.data().native(), (int)w1251_str.size().count(), 0, 0);
            if (!len) throw std::runtime_error("faild convert from cp1251 to unicode");
            std::vector<unsigned short> unicode_string(len);
            if (!MultiByteToWideChar(1251, 0, w1251_str.data().native(), (int)w1251_str.size().count(), (LPWSTR)unicode_string.data(), (int)unicode_string.size()))
                throw std::runtime_error("faild convert from cp1251 to unicode");
            len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)unicode_string.data(), (int)unicode_string.size(), 0, 0, 0, 0);
            if (!len) throw std::runtime_error("faild convert from unicode to utf-8");
            msw::buffer<char> utf8_str(len, len + (push_back_null ? 1 : 0));
            len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)unicode_string.data(), (int)unicode_string.size(), (LPSTR)utf8_str.data().native(), (int)utf8_str.size().count(), 0, 0);
            if (!len) throw std::runtime_error("faild convert from unicode to utf-8");
            if (push_back_null) utf8_str.push_back(msw::byte(0));
            return std::move(utf8_str);
        }
#endif
        msw::buffer<char> r;
        return r;
    }
}
