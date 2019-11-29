#pragma once

#include <array>
#include <type_traits>


namespace msw {
namespace zzz {

    template <typename Src, typename Dst>
    struct as_array_of
    {
        static_assert(sizeof(Src) > sizeof(Dst), "size of Src must be more then size of Dst");
        static_assert(sizeof(Src) % sizeof(Dst) == 0, "size of Dst must be even of size of Src");
        static unsigned const count = sizeof(Src) / sizeof(Dst);
        typedef std::array<Dst, count> array_type;
        typedef typename std::conditional
        <
            std::is_const<Src>::value
            , array_type const&
            , array_type&
        >::type result_type;
    };

}}

namespace msw
{
    template <typename Dst, typename Src>
    typename zzz::as_array_of<Src, Dst>::result_type as_array_of(Src& src)
    {
        return reinterpret_cast<zzz::as_array_of<Src, Dst>::result_type>(src);
    }
}
