#pragma once

#include <type_traits>                  // std::enable_if, std::remove_const, std::remove_reference

#include <msw/type_traits/is_range.hpp>


namespace msw
{
    template <typename Range>
    typename std::enable_if
    <
        is_range
        <
            typename std::remove_const
            <
                typename std::remove_reference
                <
                    typename std::remove_const<Range>::type
                >::type
            >::type
        >::value
        , Range&&
    >::type
    make_range(Range&& r)
    {
        return std::forward<Range>(r);
    }
}
