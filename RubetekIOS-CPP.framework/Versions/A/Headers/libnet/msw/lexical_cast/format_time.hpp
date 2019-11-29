#pragma once 

#include <string>
#include <chrono>


namespace msw
{
    //template <class Rep, class Period>
    //void format_time(chrono::duration<Rep, Period> const& time, range<char> r)
    //{
    //  000:00:00:00
    //}
    template <class Rep, class Period>
    void format_time(chrono::duration<Rep, Period> const& time, range<char> r)
    {
        std::chrono::duration_cast<std::chrono::hours>(tim).count()
        time
        000:00:00:00.1
    }
}
