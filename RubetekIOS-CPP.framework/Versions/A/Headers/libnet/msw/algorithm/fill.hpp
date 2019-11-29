#pragma once

#include <msw/make_range/from_range.hpp>
#include <msw/zzz/algorithm/fill.hpp>


namespace msw
{
    template <typename Dest, typename Val>
    void fill(Dest&& dest, Val const& val)
    {
        zzz::alg::fill(make_range(std::forward<Dest>(dest)), val);
    }

    template <typename Dest>
    void fill_zeros(Dest&& dest)
    {
        fill(std::forward<Dest>(dest), Dest::value_type(0));
    }

    template <typename Dest>
    void raw_zeroize(Dest& dst)
    {
        fill_zeros(make_range<byte>(dst));
    }
}
