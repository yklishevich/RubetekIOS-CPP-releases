#pragma once

#include <functional>

#include <msw/assert.hpp>
#include <msw/std/thread.hpp>
#include <msw/std/chrono.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>


namespace msw
{
    struct overseer
        : noncopyable
    {
        template <typename Fn>      overseer  (
                                                    Fn&&
                                                  , msw::chrono::steady_clock::duration alert_interval
                                                  , msw::chrono::steady_clock::duration check_interval = msw::chrono::milliseconds(100)) ;
                                    ~overseer ()                                                                                         ;
                               void mark      ()                                                                                         ;
                               void stop      ()                                                                                         ;
    private:
        msw::chrono::steady_clock::duration   const alert_interval_ ;
        msw::chrono::steady_clock::time_point       alert_time_     ;
        msw::trigger                                do_it_          ;
        thread                                      thread_         ;
    };
}


namespace msw
{
    template <typename Fn>
    overseer::overseer
    (
          Fn&& fn
        , msw::chrono::steady_clock::duration alert_interval
        , msw::chrono::steady_clock::duration check_interval
    )
        : alert_interval_(alert_interval)
        , thread_
        (
            [this, fn, check_interval]
            {
                try
                {
                    mark();
                    do_it_.set();
                    while (do_it_)
                    {
                        msw::this_thread::sleep_for(check_interval);
                        if (alert_time_ < msw::chrono::steady_clock::now()) fn();
                    }
                }
                catch (...)
                {
                    MSW_ASSERT(0);
                }
            }
        )
    {}
    inline overseer::~overseer()
    {
        stop();
        thread_.join();
    }
    inline void overseer::mark()
    {
        alert_time_ = msw::chrono::steady_clock::now() + alert_interval_;
    }
    inline void overseer::stop()
    {
        do_it_.reset();
    }
}
