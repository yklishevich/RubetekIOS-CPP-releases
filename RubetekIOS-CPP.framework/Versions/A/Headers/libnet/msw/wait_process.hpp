#pragma once

#include <msw/handle.hpp>
#include <msw/std/chrono.hpp>
#include <msw/std/thread.hpp>
#include <msw/process_is_active.hpp>


namespace msw
{
    inline bool wait_process(handle::native_type h, msw::chrono::system_clock::duration timeout)
    {
        msw::chrono::system_clock::time_point const deadline = msw::chrono::system_clock::now() + timeout;
        for (;;)
        {
            if (!process_is_active(h)) return true;
            if (msw::chrono::system_clock::now() > deadline) return false;
            msw::this_thread::sleep_for(msw::chrono::milliseconds(10));
        }
    }
}
