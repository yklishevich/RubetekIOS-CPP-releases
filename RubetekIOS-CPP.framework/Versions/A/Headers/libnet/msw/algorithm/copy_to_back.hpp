#pragma once

#include <msw/make_range/from_range.hpp>
#include <msw/zzz/algorithm/copy_to_back.hpp>


namespace msw
{
    template <typename Src, typename Dest>
    void copy_to_back(Src const& src, Dest&& dest)
    {
        zzz::alg::copy_to_back(make_range(src), make_range(std::forward<Dest>(dest)));
    }
}
