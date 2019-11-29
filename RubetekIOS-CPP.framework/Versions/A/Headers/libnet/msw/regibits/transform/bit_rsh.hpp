#pragma once

#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    struct bit_rsh
        : noncopyable
    {
        explicit bit_rsh(int count, T rest = T())
            : count_      ( count                  )
            , rest_shift_ ( sizeof(T) * 8 - count_ )
            , rest_       ( rest                   )

        {
            MSW_ASSERT(count <= static_cast<int>(sizeof(T) * 8));
        }

        T operator ()(T value)
        {
            T next_rest = value >> rest_shift_;
            value <<= count_;
            value |= rest_;
            rest_ = next_rest;
            return value;
        }

        T rest() const
        {
            return rest_;
        }

    private:

        int const count_      ;
        int const rest_shift_ ;
        T         rest_       ;
    };

}}
