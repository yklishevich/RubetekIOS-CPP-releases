#pragma once 

#include <cstring>

#include <msw/config.hpp>
#include <msw/buffer.hpp>
#include <msw/lexical_cast/int_to/dec_size.hpp>

#ifdef MSW_MSVC
#   include <msw/zzz/lexical_cast/int_to/ms_itoa.hpp>
#else
#   include <msw/zzz/lexical_cast/int_to/generic_itoa.hpp>
#endif


namespace msw
{
    template <typename T>
    position<char> int_to_dec(T v, range<char> r)
    {
        char* ptr = r.data().native();
#ifdef MSW_MSVC
        zzz::ms_itoa(v, ptr, static_cast<int>(r.size().count()));
#else
        zzz::generic_itoa(v, ptr);
#endif
        return r.begin(strlen(ptr));
    }

    template <typename T>
    buffer<char> int_to_dec(T v)
    {
        buffer<char> buf(int_to_dec_size<T>() + 1);
        buf.erase_from_to_back(int_to_dec(v, buf.all()));
        buf.push_back_silent(char(0));
        return buf;
    }
}
