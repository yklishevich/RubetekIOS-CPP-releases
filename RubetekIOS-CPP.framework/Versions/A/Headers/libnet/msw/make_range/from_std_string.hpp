#pragma once

#include <string>
#include <type_traits>                                  // std::enable_if, std::remove_const

#include <msw/type_traits/is_std_string.hpp>
#include <msw/zzz/make_range/result.hpp>
#include <msw/zzz/make_range/from_std_container.hpp>


namespace msw
{
    template <typename String>
    typename std::enable_if
    <
        is_std_string
        <
            typename std::remove_const<String>::type
        >::value
        , typename zzz::make_range_result<String const>::type
    >::type
    make_range(String const& s)
    {
        return zzz::make_range_from_std_container(s);
    }
}
