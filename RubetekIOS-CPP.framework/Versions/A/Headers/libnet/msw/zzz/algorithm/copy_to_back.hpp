#pragma once

#include <msw/assert.hpp>
#include <msw/zzz/algorithm/copy.hpp>


namespace msw {
namespace zzz {
namespace alg {

    template <typename Src, typename Dest>
    void copy_to_back(Src src, Dest dest)
    {
        MSW_ASSERT(dest.size() >= src.size());
        alg::copy(src, dest.back(src.size()));
    }

}}}
