#pragma once

#include <msw/make_range.hpp>
#include <msw/zzz/algorithm/copy.hpp>


namespace msw
{
    template <typename Src, typename Dest>
    void copy(Src const& src, Dest&& dest)
    {
        zzz::alg::copy(make_range(src), make_range(dest));
    }
}
