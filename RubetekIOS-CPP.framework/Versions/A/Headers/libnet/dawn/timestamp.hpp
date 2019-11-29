#pragma once

#include <msw/std/chrono.hpp>
#include <msw/plain_types.hpp>


namespace dawn
{
    struct timestamp
    {
        msw::uint32 sec  ;
        msw::uint32 usec ;

        timestamp()
            : sec  ( 0 )
            , usec ( 0 )
        {}
        timestamp(msw::uint32 sec, msw::uint32 usec)
            : sec  ( sec  )
            , usec ( usec )
        {}
        static timestamp now()
        {
            timestamp ts;
            auto const now_time             = msw::chrono::system_clock::now(); // warning! using only system_clock!
            auto const now_time_since_epoch = now_time.time_since_epoch();
            ts.sec  = static_cast<msw::uint32>(std::chrono::duration_cast<std::chrono::seconds>     (now_time_since_epoch).count()                                   );
            ts.usec = static_cast<msw::uint32>(std::chrono::duration_cast<std::chrono::microseconds>(now_time_since_epoch).count() - msw::int64(ts.sec) * 1000 * 1000);
            return ts;
        }
        bool operator == (timestamp const& ts)
        {
            return (ts.sec == sec) && (ts.usec == usec);
        }
        bool operator >= (timestamp const& ts)
        {
            if (sec >  ts.sec) return true             ;
            if (sec == ts.sec) return (usec >= ts.usec);
            return false;
        }
        timestamp& operator ++ ()
        {
            ++usec;
            if (!usec) ++sec;
            return *this;
        }
    };

    struct timestamp_maker
    {
        timestamp get()
        {
            timestamp const ts = timestamp::now();
            if (ts_ >= ts) ++ts_    ;
            else           ts_ = ts ;
            return ts_;
        }
    private:
        timestamp ts_;
    };
}
