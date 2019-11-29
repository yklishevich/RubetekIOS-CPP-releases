#pragma once

#include <msw/make_range/from_range.hpp>
#include <msw/zzz/algorithm/for_each.hpp>


namespace msw
{
    template <typename Dest, typename Fn>
    void for_each(Dest&& dest, Fn&& fn)
    {
        zzz::alg::for_each(make_range(std::forward<Dest>(dest)), std::forward<Fn>(fn));
    }
}
