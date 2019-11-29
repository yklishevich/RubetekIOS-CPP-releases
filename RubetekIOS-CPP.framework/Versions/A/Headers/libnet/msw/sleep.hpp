#pragma once

#include <msw/std/chrono.hpp>
#include <msw/std/thread.hpp>


namespace msw
{
    inline void sleep(msw::uint sec)
    {
        this_thread::sleep_for(chrono::seconds(sec));
    }
}
