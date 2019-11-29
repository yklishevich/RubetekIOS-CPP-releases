#pragma once

#include <msw/config.hpp>
#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
#   include <msw/safe_bool.hpp>
#endif


namespace msw
{
    struct trigger
    {
        explicit trigger                 (bool state = false)       ;

                 trigger                 (trigger const&)           ;
        trigger& operator =              (trigger const&)           ;

                 trigger                 (trigger&&)                ;
        trigger& operator =              (trigger&&)                ;

        void     set                     (bool = true)              ;
        void     reset                   ()                         ;
        void     invert                  ()                         ;

#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
        typedef safe_bool<trigger>::type safe_bool_type;
                 operator safe_bool_type ()                   const ;
#else
        explicit operator bool           ()                   const ;
#endif

    private:
        bool state_;
    };
}


namespace msw
{
    inline trigger::trigger(bool state)
        : state_(state)
    {}

    inline trigger::trigger(trigger const& other)
        : state_(other.state_)
    {}
    inline trigger& trigger::operator = (trigger const& other)
    {
        state_ = other.state_;
        return *this;
    }

    inline trigger::trigger(trigger&& other)
        : state_(other.state_)
    {
        other.state_ = false;
    }
    inline trigger& trigger::operator = (trigger&& other)
    {
        state_ = other.state_;
        other.state_ = false;
        return *this;
    }

    inline void trigger::set(bool state)
    {
        state_ = state;
    }
    inline void trigger::reset()
    {
        state_ = false;
    }
    inline void trigger::invert()
    {
        state_ = !state_;
    }

#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
    inline trigger::operator safe_bool_type () const
#else
    inline trigger::operator bool() const
#endif
    {
        return
#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
            make_safe_bool<trigger>(state_);
#else
            state_;
#endif
    }
}
