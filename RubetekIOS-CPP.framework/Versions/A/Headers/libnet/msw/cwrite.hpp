#pragma once

#include <iostream>

#include <msw/format.hpp>


namespace msw
{
    template <typename T>
    void cwrite(T&& v)
    {
        format_s(std::cout, std::forward<T>(v));
    }
    template <typename T, typename ...Args>
    void cwrite(T&& v, Args&& ...args)
    {
        cwrite(std::forward<T>(v));
        cwrite(std::forward<Args>(args)...);
    }

    inline void cwriteln()
    {
        cwrite(nl);
    }
    template <typename T>
    void cwriteln(T&& v)
    {
        cwrite(std::forward<T>(v));
        cwriteln();
    }
    template <typename T, typename ...Args>
    void cwriteln(T&& v, Args&& ...args)
    {
        cwrite(std::forward<T>(v));
        cwrite(std::forward<Args>(args)...);
        cwriteln();
    }

    inline void cflush()
    {
        std::cout.flush();
    }

    template <typename ...Args>
    void cwritef(Args&& ...args)
    {
        cwrite(std::forward<Args>(args)...);
        cflush();
    }
    template <typename ...Args>
    void cwritelnf(Args&& ...args)
    {
        cwriteln(std::forward<Args>(args)...);
        cflush();
    }
}
