#pragma once

#include <vector>
#include <type_traits>                                  // std::enable_if, std::remove_const

#include <msw/type_traits/is_std_vector.hpp>
#include <msw/zzz/make_range/result.hpp>
#include <msw/zzz/make_range/from_std_container.hpp>


namespace msw
{
    template <typename Vector>
    typename std::enable_if
    <
        is_std_vector
        <
            typename std::remove_const
            <
                typename std::remove_reference<Vector>::type
            >::type
        >::value
        , typename zzz::make_range_result<Vector>::type
    >::type
    make_range(Vector&& v)
    {
        return zzz::make_range_from_std_container(std::forward<Vector>(v));
    }
}
