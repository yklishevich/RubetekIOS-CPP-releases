#pragma once

#include <type_traits>

#include <msw/type_traits/is_bit_plain.hpp>
#include <msw/type_traits/is_byte_plain.hpp>
#include <msw/regibits/permutation/fill_group.hpp>
#include <msw/regibits/permutation/fill_lsb_group.hpp>
#include <msw/regibits/permutation/fill_msb_group.hpp>


namespace msw {
namespace zzz {
namespace alg {

    template <typename Dest, typename Val>
    typename std::enable_if<is_byte_plain<typename Dest::value_type>::value>::type
    fill(Dest dest, Val const& val)
    {
        auto first = dest.begin().to_pointer().native();
        auto last = dest.end().to_pointer().native();
        for (; first != last; ++first) *first = val;
    }

    template <typename Dest, typename Val>
    typename std::enable_if<is_bit_plain<typename Dest::value_type>::value>::type
    fill(Dest dest, Val const& val)
    {
        auto prow_size = dest.begin().to_pointer().reverse_offset();
        if (dest.begin().to_pointer().base() == dest.end().to_pointer().base())
        {
            regibits::fill_group_n(*dest.begin().to_pointer().base(), prow_size, static_cast<unsigned>(dest.size().count()), static_cast<bool>(val));
        }
        else
        {
            regibits::fill_msb_group(*dest.begin().to_pointer().base(), prow_size, static_cast<bool>(val));
            dest.pop_front_max(prow_size);
            alg::fill(dest.template pop_front_max<bit::base_type>(), bit::base_type(val ? -1 : 0));
            regibits::fill_lsb_group(*dest.end().to_pointer().base(), dest.end().to_pointer().offset(), static_cast<bool>(val));
        }
    }

}}}
