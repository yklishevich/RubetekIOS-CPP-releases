#pragma once

#include <functional>

#include <msw/noncopyable.hpp>
#include <msw/std/chrono.hpp>
#include <msw/callback/event.hpp>


namespace rubetek
{
    struct passive_timer
        : msw::noncopyable
    {
        typedef msw::callback::event event;

        template <typename Fn>
        passive_timer(Fn&& fn, msw::chrono::steady_clock::duration interval)
            : event_     ( std::forward<Fn>(fn)             )
            , interval_  ( interval                         )
            , next_call_ ( msw::chrono::steady_clock::now() )
        {}

        void idle()
        {
            if (msw::chrono::steady_clock::now() >= next_call_) call();
        }

        void set_interval(msw::chrono::steady_clock::duration interval, bool perform = true)
        {
            interval_ = interval;
            if (perform) call();
        }

        void reset()
        {
            next_call_ = msw::chrono::steady_clock::now() + interval_;
        }

    private:

        void call()
        {
            event_();
            reset();
        }

        event                                 event_     ;
        msw::chrono::steady_clock::duration   interval_  ;
        msw::chrono::steady_clock::time_point next_call_ ;
    };
}
