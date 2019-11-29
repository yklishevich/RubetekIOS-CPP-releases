#pragma once

#include <msw/size.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/odometer.hpp>
#include <msw/dimension/speedometer.hpp>


namespace msw
{
    struct multimeter
        : noncopyable
    {
        typedef msw::speedometer<>::value_type speed_type    ;
        typedef msw::odometer<>::value_type    distance_type ;

        struct snapshot
        {
            distance_type distance ;
            speed_type    speed    ;
        };

        void inc_distance()
        {
            odometer_.add(1);
            speedometer_.inc_distance();
        }
        void add_distance(distance_type value)
        {
            odometer_.add(value);
            speedometer_.add_distance(static_cast<speed_type>(value));
        }
        template <typename T>
        void add_distance(size<T> value)
        {
            add_distance(value.count());
        }
        void set_distance(distance_type value)
        {
            MSW_ASSERT(value >= odometer_.value());
            distance_type const distance = value - odometer_.value();
            add_distance(distance);
        }
        template <typename T>
        void set_distance(size<T> value)
        {
            set_distance(value.count());
        }

        bool empty() const
        {
            return odometer_.empty();
        }

        distance_type distance() const
        {
            return odometer_.value();
        }

        speed_type speed()
        {
            return speedometer_.speed();
        }

        snapshot make_snapshot()
        {
            return snapshot({distance(), speed()});
        }

        void reset()
        {
            odometer_.reset();
            speedometer_.reset();
        }

    private:

        msw::odometer<>    odometer_    ;
        msw::speedometer<> speedometer_ ;
    };
}
