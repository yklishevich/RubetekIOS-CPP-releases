#pragma once

#include <msw/make_range/from_range.hpp>
#include <msw/zzz/algorithm/generate.hpp>


namespace msw
{
    template <typename Dest, typename Generator>
    void generate(Dest&& dest, Generator&& gen)
    {
        zzz::alg::generate(make_range(std::forward<Dest>(dest)), std::forward<Generator>(gen));
    }
}
