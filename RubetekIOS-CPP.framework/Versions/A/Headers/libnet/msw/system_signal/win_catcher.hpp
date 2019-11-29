#pragma once

#include <msw/std/mutex.hpp>
#include <msw/console/win.hpp>
#include <msw/cross_thread/event.hpp>


namespace msw
{
    struct win_signal_catcher
    {
        explicit win_signal_catcher () ;
        ~        win_signal_catcher () ;
        int      check_signal       () ;
    };
};


namespace msw     {
namespace zzz     {
namespace sys_sig {

    static int                 sig = -1 ;
    static msw::mutex          mtx      ;
    static cross_thread::event closing  ;

    inline bool __stdcall handler_routine(DWORD s)
    {
        using namespace zzz::sys_sig;
        {
            msw::unique_lock<msw::mutex> lock(mtx);
            sig = s;
        }
        if (s >= CTRL_CLOSE_EVENT) closing.wait();
        return true;
    };

}}}


namespace msw
{
    inline win_signal_catcher::win_signal_catcher()
    {
        console::win::set_ctrl_handler(zzz::sys_sig::handler_routine);
    }
    inline win_signal_catcher::~win_signal_catcher()
    {
        zzz::sys_sig::closing.notify();
    }

    inline int win_signal_catcher::check_signal()
    {
        using namespace zzz::sys_sig;
        msw::unique_lock<msw::mutex> lock(mtx);
        int s = sig;
        sig = -1;
        return s;
    }
};
