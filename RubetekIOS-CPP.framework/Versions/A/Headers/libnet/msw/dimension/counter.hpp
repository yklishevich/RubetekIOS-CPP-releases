#pragma once

#include <msw/int.hpp>
#include <msw/assert.hpp>


namespace msw
{
    template <typename Val = sint, Val Step = 1>
    struct counter
    {
        typedef counter<Val, Step> self_type  ;
        typedef Val                value_type ;

        explicit   counter     (value_type value = 0)       ;

        self_type& operator ++ ()                           ;
        self_type& operator -- ()                           ;
        self_type  operator ++ (int)                        ;
        self_type  operator -- (int)                        ;

        self_type& operator += (value_type value)           ;
        self_type& operator -= (value_type value)           ;

        value_type value       ()                     const ;
        void       set         (value_type value)           ;
        void       reset       ()                           ;

    private:
        value_type value_;
    };
}


namespace msw
{
    template <typename Val, Val Step>
    counter<Val, Step>::counter(value_type value)
        : value_(value)
    {}

    template <typename Val, Val Step>
    typename counter<Val, Step>::self_type& counter<Val, Step>::operator ++ ()
    {
        value_ += Step;
        return *this;
    }
    template <typename Val, Val Step>
    typename counter<Val, Step>::self_type& counter<Val, Step>::operator -- ()
    {
        value_ -= Step;
        return *this;
    }
    template <typename Val, Val Step>
    typename counter<Val, Step>::self_type counter<Val, Step>::operator ++ (int)
    {
        self_type resutl(*this);
        ++(*this);
        return resutl;
    }
    template <typename Val, Val Step>
    typename counter<Val, Step>::self_type counter<Val, Step>::operator -- (int)
    {
        self_type resutl(*this);
        --(*this);
        return resutl;
    }

    template <typename Val, Val Step>
    typename counter<Val, Step>::self_type& counter<Val, Step>::operator += (value_type value)
    {
        MSW_ASSERT((value % Step) == 0);
        value_ += value;
        return *this;
    }
    template <typename Val, Val Step>
    typename counter<Val, Step>::self_type& counter<Val, Step>::operator -= (value_type value)
    {
        MSW_ASSERT((value % Step) == 0);
        value_ -= value;
        return *this;
    }

    template <typename Val, Val Step>
    typename counter<Val, Step>::value_type counter<Val, Step>::value() const
    {
        return value_;
    }
    template <typename Val, Val Step>
    void counter<Val, Step>::set(value_type value)
    {
        MSW_ASSERT((value % Step) == 0);
        value_ = value;
    }
    template <typename Val, Val Step>
    void counter<Val, Step>::reset()
    {
        value_ = value_type();
    }
}
