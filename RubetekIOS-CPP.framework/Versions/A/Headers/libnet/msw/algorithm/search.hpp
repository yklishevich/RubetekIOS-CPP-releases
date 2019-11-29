#pragma once

#include <type_traits>                          // std::remove_const, std::enable_if

#include <msw/type_traits/is_plain.hpp>
#include <msw/make_range/from_range.hpp>
#include <msw/type_traits/remove_reference.hpp>
#include <msw/zzz/algorithm/search.hpp>
#include <msw/zzz/make_range/result.hpp>


namespace msw
{
    template <typename What, typename Where>
    typename std::enable_if
    <
        is_plain
        <
            typename std::remove_const
            <
                typename remove_reference<What>::type
            >::type
        >::value
        , typename zzz::make_range_result<Where>::type::position
    >::type
    search(What const& what, Where&& where)
    {
        return zzz::alg::search_value(what, make_range(std::forward<Where>(where)));
    }

    template <typename What, typename Where>
    typename std::enable_if
    <
        !is_plain
        <
            typename std::remove_const
            <
                typename remove_reference<What>::type
            >::type
        >::value
        , typename zzz::make_range_result<Where>::type::position
    >::type
    search(What const& what, Where&& where)
    {
        return zzz::alg::search_range(what, make_range(std::forward<Where>(where)));
    }
}
