#pragma once


namespace msw {
namespace zzz {
namespace alg {

    template <typename Dest, typename Fn>
    void for_each(Dest dest, Fn&& fn)
    {
        auto first = dest.begin().to_pointer();
        auto last = dest.end().to_pointer();
        for (; first != last; ++first) fn(*first);
    }

}}}
