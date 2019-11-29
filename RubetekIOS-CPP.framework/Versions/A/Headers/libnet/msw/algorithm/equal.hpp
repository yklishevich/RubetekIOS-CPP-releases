#pragma once

#include <msw/make_range.hpp>
#include <msw/zzz/algorithm/equal.hpp>


namespace msw
{
    template <typename X1, typename X2>
    bool equal(X1 const& x1, X2 const& x2)
    {
        return zzz::alg::equal(make_range(x1), make_range(x2));
    }
    template <typename X1, typename X2>
    bool partial_equal(X1 const& x1, X2 const& x2)
    {
        return zzz::alg::partial_equal(make_range(x1), make_range(x2));
    }
}
