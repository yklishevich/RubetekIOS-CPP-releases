#pragma once

#include <msw/std/chrono.hpp>


namespace dawn
{
    struct restart_profile
    {
        unsigned                            max_unstable_restarts         ;
        msw::chrono::steady_clock::duration interval_unstable_restart     ;
        msw::chrono::steady_clock::duration time_stable_work              ;
        msw::chrono::steady_clock::duration pause_after_unstable_restarts ;

             restart_profile ()                             ;
        bool operator    ==  (restart_profile const&) const ;
    };
}


namespace dawn
{
    inline restart_profile::restart_profile()
        : max_unstable_restarts(0)
    {}

    inline bool restart_profile::operator == (restart_profile const& other) const
    {
        return (max_unstable_restarts         == other.max_unstable_restarts         )
            && (interval_unstable_restart     == other.interval_unstable_restart     )
            && (time_stable_work              == other.time_stable_work              )
            && (pause_after_unstable_restarts == other.pause_after_unstable_restarts )
        ;
    }
}
