#pragma once

#include <string>
#include <cstring>        // std::memcpy
#include <type_traits>

#include <msw/config.hpp>


namespace msw {
namespace zzz {

    inline void generic_itoa(int v, char* buf)
    {
        std::string s =
#ifdef MSW_NO_COMPLETE_TO_STRING
            std::to_string(static_cast<long long int>(v));
#else
            std::to_string(v);
#endif
        std::memcpy(buf, s.c_str(), s.size() + 1);
    }

    template <typename T>
    typename std::enable_if<sizeof(T) < 4>::type
    generic_itoa(T v, char* buf)
    {
        generic_itoa(static_cast<int>(v), buf);
    }

    template <typename T>
    typename std::enable_if<sizeof(T) >= 4>::type
    generic_itoa(T v, char* buf)
    {
        static_assert(!std::is_same<T, int>::value, "T should not be int");
        std::string s = std::to_string(v);
        std::memcpy(buf, s.c_str(), s.size() + 1);
    }

}}
