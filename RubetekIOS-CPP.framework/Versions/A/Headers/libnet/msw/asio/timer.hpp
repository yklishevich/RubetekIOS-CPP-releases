#pragma once

#include <memory>

#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/zzz/asio/timer.hpp>


namespace msw
{
    struct timer
        : noncopyable
    {
        typedef zzz::timer::io_service io_service ;
        typedef zzz::timer::on_tick    on_tick    ;
        typedef zzz::timer::duration   duration   ;

                    timer          (io_service&, duration, on_tick, on_error = nullptr)       ;
                    ~timer         ()                                                         ;
        duration    interval       ()                                                   const ;
        io_service& get_io_service ()                                                         ;
    private:
        std::shared_ptr<zzz::timer> timer_;
    };
}


namespace msw
{
    inline timer::timer(io_service& io_service, duration interval, on_tick on_tick, on_error on_error)
        : timer_(std::make_shared<zzz::timer>(io_service, interval, on_tick, on_error))
    {
        timer_->start();
    }
    inline timer::~timer()
    {
        timer_->cancel();
    }
    inline timer::duration timer::interval() const
    {
        return timer_->interval();
    }
    inline timer::io_service& timer::get_io_service()
    {
        return timer_->get_io_service();
    }
}
