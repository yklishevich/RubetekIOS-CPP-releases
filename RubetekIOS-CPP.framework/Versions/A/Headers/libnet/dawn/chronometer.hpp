#pragma once

#include <string>

#include <lama/boost/include/date_time/posix_time/posix_time.hpp>


namespace dawn
{
    struct chronometer
    {
                                         chronometer     (bool microsec = false)       ;
        boost::posix_time::time_duration duration        ()                      const ;
        std::string                      duration_string ()                      const ;
    private:
        boost::posix_time::ptime         local_time      ()                      const ;

        bool                     const microsec_ ;
        boost::posix_time::ptime const start_    ;
    };
}


namespace dawn
{
    inline chronometer::chronometer(bool microsec)
        : microsec_ ( microsec     )
        , start_    ( local_time() )
    {}
    inline boost::posix_time::time_duration chronometer::duration() const
    {
        return local_time() - start_;
    }
    inline std::string chronometer::duration_string() const
    {
        return boost::posix_time::to_simple_string(duration());
    }

    inline boost::posix_time::ptime chronometer::local_time() const
    {
        return microsec_ ? boost::posix_time::microsec_clock::local_time() : boost::posix_time::second_clock::local_time();
    }
}
