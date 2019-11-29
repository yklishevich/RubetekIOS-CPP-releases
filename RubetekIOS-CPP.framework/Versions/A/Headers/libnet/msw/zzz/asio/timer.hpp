#pragma once

#include <memory>
#include <functional>

#include <msw/noncopyable.hpp>
#include <msw/std/chrono.hpp>
#include <msw/zzz/asio/error_handler.hpp>

#include <boost/asio/steady_timer.hpp>


namespace msw {
namespace zzz {

    struct timer
        : noncopyable
        , std::enable_shared_from_this<timer>
    {
        typedef boost::asio::io_service             io_service ;
        typedef std::function<void()>               on_tick    ;
        typedef msw::chrono::steady_clock::duration duration   ;

                    timer          (io_service&, duration, on_tick, on_error)       ;
        void        start          ()                                               ;
        void        cancel         ()                                               ;
        duration    interval       ()                                         const ;
        io_service& get_io_service ()                                               ;

    private:
        void        wait           ()                                               ;

        duration                  const interval_      ;
        boost::asio::steady_timer       timer_         ;
        on_tick                         on_tick_       ;
        error_handler                   error_handler_ ;
    };

}}


namespace msw {
namespace zzz {

    inline timer::timer(io_service& io_service, duration interval, on_tick on_tick, on_error on_error)
        : interval_      ( interval   )
        , timer_         ( io_service )
        , on_tick_       ( on_tick    )
        , error_handler_ ( on_error   )
    {}
    inline void timer::start()
    {
        wait();
    }
    inline void timer::cancel()
    {
        on_tick_ = nullptr;
        timer_.cancel();
    }
    inline timer::duration timer::interval() const
    {
        return interval_;
    }
    inline timer::io_service& timer::get_io_service()
    {
        return timer_.get_io_service();
    }

    inline void timer::wait()
    {
        timer_.expires_from_now(interval_);
        auto self(shared_from_this());
        timer_.async_wait([this, self](boost::system::error_code const& ec)
        {
            if (on_tick_)
            {
                if (ec) error_handler_(ec);
                else (on_tick(on_tick_))();
                if (on_tick_) wait();
            }
        });
    }

}}
