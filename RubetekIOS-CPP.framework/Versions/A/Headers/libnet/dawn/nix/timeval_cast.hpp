#pragma once

#include <msw/plain_types.hpp>
#include <msw/std/chrono.hpp>


namespace dawn {
namespace nix  {

    template <typename TimeVal>
    chrono::microseconds timeval_to_chrono(TimeVal const& tv)
    {
        return chrono::seconds(tv.sec) + chrono::microseconds(tv.usec);
    }

    template <typename TimeVal, typename Duration>
    TimeVal timeval_from_chrono(Duration duration)
    {
        uint64 const seconds_count = chrono::duration_cast<chrono::seconds>(duration).count();
        uint64 const microseconds_count = chrono::duration_cast<chrono::microseconds>(duration).count();
        return TimeVal({static_cast<uint32>(seconds_count), static_cast<uint32>(microseconds_count - seconds_count * 1000000)});
    }

}}
