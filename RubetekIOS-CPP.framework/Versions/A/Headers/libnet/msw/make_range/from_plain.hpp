#pragma once

#include <type_traits>                      // std::enable_if, std::remove_const

#include <msw/config.hpp>
#include <msw/plain_traits.hpp>
#include <msw/type_traits/is_plain.hpp>
#include <msw/make_range/from_pointers.hpp>
#include <msw/zzz/ptr/make.hpp>
#include <msw/zzz/make_range/result.hpp>


namespace msw
{
    template <typename Plain>
    typename std::enable_if
    <
        is_plain
        <
            typename std::remove_const
            <
                typename std::remove_reference<Plain>::type
            >::type
        >::value
        , typename zzz::make_range_result<Plain>::type
    >::type
    make_range(Plain&& v)
    {
        typedef typename std::remove_const
        <
            typename std::remove_reference<Plain>::type
        >::type value_type;
        return make_range(make_ptr(&v), typename plain_traits<value_type>::size_type(1));
    }
}
