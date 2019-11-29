#pragma once

#include <functional>

#include <msw/std/chrono.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/counter.hpp>
#include <msw/asio/mutable_timer.hpp>
#include <msw/throw_runtime_error.hpp>

#include <dawn/restart/profile.hpp>


namespace dawn
{
    struct manager_restarts
        : msw::noncopyable
    {
        typedef std::function<void()> on_restart;

        explicit manager_restarts (boost::asio::io_service&, restart_profile const&, on_restart)       ;

        void     started          ()                                                                   ;
        void     completed        ()                                                                   ;

        bool     is_started       ()                                                             const ;

    private:
        void     restart          ()                                                                   ;

        restart_profile        const profile_       ;
        msw::trigger                 started_       ;
        on_restart                   on_restart_    ;
        msw::counter<unsigned>       starts_count_  ;
        msw::mutable_timer           mutable_timer_ ;
    };
}


namespace dawn
{
    inline manager_restarts::manager_restarts(boost::asio::io_service& io_service, restart_profile const& profile, on_restart on_restart)
        : profile_       ( profile    )
        , started_       ( false      )
        , on_restart_    ( on_restart )
        , mutable_timer_ ( io_service )
    {
        io_service.post
        (
            [this]()
            {
                restart();
            }
        );
    }

    inline void manager_restarts::started()
    {
        if (is_started()) msw::throw_runtime_error("application is already started");
        started_.set();
        mutable_timer_.start_from_now
        (
            profile_.time_stable_work
            , [this]()
            {
                starts_count_.reset();
            }
        );
    }
    inline void manager_restarts::completed()
    {
        if (!is_started()) msw::throw_runtime_error("application is already completed");
        started_.reset();
        restart();
    }

    inline bool manager_restarts::is_started() const
    {
        return static_cast<bool>(started_);
    }

    inline void manager_restarts::restart()
    {
        if (starts_count_.value() == (profile_.max_unstable_restarts + 1))
        {
            mutable_timer_.start_from_now
            (
                profile_.pause_after_unstable_restarts
                , [this]()
                {
                    starts_count_.reset();
                    restart();
                }
            );
        }
        else
        {
            on_restart_();
            starts_count_++;
            mutable_timer_.start_from_now
            (
                profile_.interval_unstable_restart
                , [this]()
                {
                    restart();
                }
            );
        }
    }
}
