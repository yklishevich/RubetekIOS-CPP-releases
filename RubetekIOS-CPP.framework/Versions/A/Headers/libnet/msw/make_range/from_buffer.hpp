#pragma once

#include <type_traits>                   // std::enable_if, std::remove_const

#include <msw/type_traits/is_buffer.hpp>
#include <msw/zzz/buffer/decl.hpp>
#include <msw/zzz/make_range/result.hpp>


namespace msw
{
    template <typename Buffer>
    typename std::enable_if
    <
        is_buffer
        <
            typename std::remove_const
            <
                typename std::remove_reference<Buffer>::type
            >::type
        >::value
        , typename zzz::make_range_result<Buffer>::type
    >::type
    make_range(Buffer&& buf)
    {
        return buf.all();
    }
}
