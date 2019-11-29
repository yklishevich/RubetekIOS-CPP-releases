#pragma once


namespace msw {
namespace zzz {
namespace alg {

    template <typename Dest, typename Generator>
    void generate(Dest dest, Generator&& gen)
    {
        auto first = dest.begin().to_pointer().native();
        auto const last = dest.end().to_pointer().native();
        for (;first != last; ++first) *first = gen();
    }

}}}
