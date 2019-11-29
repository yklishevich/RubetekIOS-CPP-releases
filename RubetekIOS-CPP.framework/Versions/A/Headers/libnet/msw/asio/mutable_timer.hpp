#pragma once

#include <memory>

#include <msw/std/memory.hpp>
#include <msw/asio/timer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw
{
    struct mutable_timer
        : noncopyable
    {
        typedef timer::on_tick  on_tick  ;
        typedef timer::duration duration ;

        explicit mutable_timer  (boost::asio::io_service&)                                              ;
                 mutable_timer  (boost::asio::io_service&, duration, on_tick, on_error = nullptr)       ;
        void     start_from_now (duration)                                                              ;
        void     start_from_now (duration, on_tick)                                                     ;
        void     start_from_now (duration, on_tick, on_error)                                           ;
        void     stop           ()                                                                      ;
        bool     expects        ()                                                                const ;
        duration interval       ()                                                                const ;
    private:
        boost::asio::io_service& io_service_ ;
        on_tick                  on_tick_    ;
        on_error                 on_error_   ;
        std::unique_ptr<timer>   timer_      ;
    };
}


namespace msw
{
    inline mutable_timer::mutable_timer(boost::asio::io_service& io_service)
        : io_service_(io_service)
    {}
    inline mutable_timer::mutable_timer(boost::asio::io_service& io_service, duration interval, on_tick on_tick, on_error on_error)
        : io_service_ ( io_service )
        , on_tick_    ( on_tick    )
        , on_error_   ( on_error   )
    {
        start_from_now(interval);
    }
    inline void mutable_timer::start_from_now(duration interval)
    {
        if (!on_tick_) throw_runtime_error("mutable_timer: on_tick is null");
        timer_ = msw::make_unique<timer>(io_service_, interval, on_tick_, on_error_);
    }
    inline void mutable_timer::start_from_now(duration interval, on_tick on_tick)
    {
        on_tick_ = on_tick;
        start_from_now(interval);
    }
    inline void mutable_timer::start_from_now(duration interval, on_tick on_tick, on_error on_error)
    {
        on_tick_ = on_tick;
        on_error_ = on_error;
        start_from_now(interval);
    }
    inline void mutable_timer::stop()
    {
        timer_.reset();
    }
    inline bool mutable_timer::expects() const
    {
        return static_cast<bool>(timer_);
    }
    inline mutable_timer::duration mutable_timer::interval() const
    {
        if (!timer_) throw_runtime_error("mutable_timer is stopped");
        return timer_->interval();
    }
}
