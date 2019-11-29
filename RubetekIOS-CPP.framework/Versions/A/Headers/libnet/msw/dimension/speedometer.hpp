#pragma once

#include <deque>
#include <chrono>
#include <algorithm>

#include <msw/int.hpp>
#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/odometer.hpp>


namespace msw
{
    template <typename Val = uint>
    struct speedometer
        : noncopyable
    {
        typedef Val                                value_type ;
        typedef std::chrono::high_resolution_clock clock_type ;
        typedef std::chrono::milliseconds          time_type  ;

        explicit   speedometer  (std::size_t capacity = 8, clock_type::duration measure_interval = time_type(200));

        void       add_distance (value_type value) ;
        void       inc_distance ()                 ;

        value_type speed        ()                 ;

        void       reset        ()                 ;

    private:

        struct point
        {
            value_type             value ;
            clock_type::time_point time  ;

            explicit point
            (
                value_type value
            )
                : value ( value             )
                , time  ( clock_type::now() )
            {}
        };

        void        calculation ()                                      ;
        value_type  calc_speed  (point const& first, point const& last) ;

        long long              const coefficient_       ;
        clock_type::duration   const measure_interval_  ;
        clock_type::time_point       last_measure_time_ ;
        msw::odometer<Val>           local_distance_    ;
        std::deque<point>            history_           ;
    };
}


namespace msw
{
    template <typename Val>
    speedometer<Val>::speedometer(std::size_t capacity, clock_type::duration measure_interval)
        : coefficient_     ( std::chrono::duration_cast<time_type>(std::chrono::seconds(1)).count() )
        , measure_interval_( measure_interval                                                       )
    {
        if (!std::chrono::duration_cast<time_type>(measure_interval_).count())
            throw std::runtime_error("very small measure interval");
        if (capacity < 2) 
            throw std::runtime_error("speedometer capacity must not less 2");
        for (; capacity; --capacity)
            history_.push_back(point(0));
    }

    template <typename Val>
    void speedometer<Val>::add_distance(value_type value)
    {
        MSW_ASSERT(value >= 0);
        local_distance_.add(value);
        calculation();
    }
    template <typename Val>
    void speedometer<Val>::inc_distance()
    {
        add_distance(1);
    }

    template <typename Val>
    typename speedometer<Val>::value_type speedometer<Val>::speed()
    {
        calculation();
        return calc_speed(history_.front(), history_.back());
    }

    template <typename Val>
    void speedometer<Val>::reset()
    {
        std::fill(history_.begin(), history_.end(), point(0));
        local_distance_.reset();
    }

    template <typename Val>
    void speedometer<Val>::calculation()
    {
        clock_type::time_point const measure_time = clock_type::now();
        clock_type::duration const interval = measure_time - last_measure_time_;
        unsigned long long cycles_count = interval.count() / measure_interval_.count();
        if (cycles_count >= history_.size()) cycles_count = history_.size();
        if (cycles_count) last_measure_time_ = measure_time;
        for (; cycles_count; --cycles_count)
        {
            value_type const emitted_value = history_.front().value;
            history_.pop_front();
            history_.push_back(point(history_.back().value + local_distance_.value()));
            local_distance_.reset();
            for (point& p : history_) p.value -= emitted_value;
        }
    }
    template <typename Val>
    typename speedometer<Val>::value_type speedometer<Val>::calc_speed(point const& first, point const& last)
    {
        auto const interval = std::chrono::duration_cast<time_type>(last.time - first.time).count();
        auto const variance = last.value - first.value;
        MSW_ASSERT(interval >= 0);
        MSW_ASSERT(variance >= 0);
        return interval ? static_cast<value_type>(variance * coefficient_ / interval) : 0;
    }
}
