#pragma once


namespace msw {
namespace zzz {
namespace alg {

    template <typename Src, typename Dest, typename UnaryOperator>
    void transform(Src src, Dest dest, UnaryOperator&& op)
    {
        MSW_ASSERT(dest.size() >= src.size());

        auto first = src.begin().to_pointer().native();
        auto const last = src.end().to_pointer().native();
        auto first_dest = dest.begin().to_pointer().native();
        for (;first != last; ++first) *first_dest++ = op(*first);
    }

}}}
