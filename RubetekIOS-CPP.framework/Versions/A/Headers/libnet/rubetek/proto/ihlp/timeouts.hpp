#pragma once

#include <msw/std/chrono.hpp>


namespace rubetek {
namespace ihlp    {

    struct timeouts
    {
        msw::chrono::steady_clock::duration response   ;
        msw::chrono::steady_clock::duration keep_alive ;

        timeouts (                                                                                              ) ;
        timeouts ( msw::chrono::steady_clock::duration response, msw::chrono::steady_clock::duration keep_alive ) ;

        bool operator == (timeouts const&) const;
        bool operator != (timeouts const&) const;
    };

}}


namespace rubetek {
namespace ihlp    {

    inline timeouts::timeouts()
        : response   ( msw::chrono::seconds (5 ) )
        , keep_alive ( msw::chrono::minutes (1 ) )
    {}
    inline timeouts::timeouts(msw::chrono::steady_clock::duration response_, msw::chrono::steady_clock::duration keep_alive_)
        : response   ( response_   )
        , keep_alive ( keep_alive_ )
    {}

    inline bool timeouts::operator == (timeouts const& other) const
    {
        return (response == other.response) && (keep_alive == other.keep_alive);
    }
    inline bool timeouts::operator != (timeouts const& other) const
    {
        return (response != other.response) || (keep_alive != other.keep_alive);
    }

}}
