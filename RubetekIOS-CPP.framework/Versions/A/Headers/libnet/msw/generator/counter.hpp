#pragma once


namespace msw       {
namespace generator {

    template <typename T>
    struct counter
    {
        typedef T value_type;

        explicit   counter    (value_type initial_state = value_type()) ;
        value_type operator() ()                                        ;
    private:
        value_type value_;
    };

}}


namespace msw       {
namespace generator {

    template <typename T>
    counter<T>::counter(value_type initial_state)
        : value_(initial_state)
    {}
    template <typename T>
    typename counter<T>::value_type counter<T>::operator() ()
    {
        return value_++;
    }

}}
