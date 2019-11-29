#pragma once

#include <msw/make_range.hpp>
#include <msw/zzz/algorithm/transform.hpp>


namespace msw
{
    template <typename Src, typename Dest, typename UnaryOperator>
    void transform(Src const& src, Dest&& dest, UnaryOperator&& op)
    {
        zzz::alg::transform(make_range(src), make_range(std::forward<Dest>(dest)), std::forward<UnaryOperator>(op));
    }
}
