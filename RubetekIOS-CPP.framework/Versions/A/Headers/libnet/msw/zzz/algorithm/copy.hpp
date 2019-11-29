#pragma once

#include <cstring>
#include <type_traits>

#include <msw/assert.hpp>
#include <msw/type_traits/is_bit_plain.hpp>
#include <msw/type_traits/is_byte_plain.hpp>
#include <msw/regibits/transform/bit_rsh.hpp>
#include <msw/regibits/permutation/apply_lsb_mask.hpp>
#include <msw/regibits/permutation/reset_lsb_group.hpp>


namespace msw {
namespace zzz {
namespace alg {

    template <typename Src, typename Dest>
    typename std::enable_if<is_byte_plain<typename Src::value_type>::value>::type
    copy(Src src, Dest dst)
    {
        MSW_ASSERT(dst.size() >= src.size());
        std::memmove(dst.begin().to_pointer().native(), src.begin().to_pointer().native(), size<byte>(src.size()).count());
    }

    template <typename Src, typename Dest>
    typename std::enable_if<is_bit_plain<typename Src::value_type>::value>::type
    copy(Src src, Dest dst)
    {
        dst.crop_front(src.size());

        auto const src_offset = src.begin().to_pointer().offset();
        auto const dst_offset = dst.begin().to_pointer().offset();
        auto const src_tail_size = src.end().to_pointer().offset();
        auto const dst_tail_size = dst.end().to_pointer().offset();

        auto src_base_pos = src.begin().to_pointer().base();
        auto dst_base_pos = dst.begin().to_pointer().base();
        auto const src_base_end = src.end().to_pointer().base();
        auto const dst_base_end = dst.end().to_pointer().base();

        int offset = 0;
        byte rest = 0;
        if (src_offset > dst_offset)
        {
            int rest_offset = src_offset - dst_offset;
            rest = *src_base_pos++ >> rest_offset;
            offset = 8 - rest_offset;
        }
        else offset = dst_offset - src_offset;
        regibits::bit_rsh<byte> shifter(offset, rest);

        byte before_prow = 0;
        if (dst_offset)
        {
            before_prow = *dst_base_pos;
            regibits::apply_lsb_mask(before_prow, dst_offset);
        }

        byte after_tail = 0;
        if (dst_tail_size)
        {
            after_tail = *dst_base_end;
            regibits::reset_lsb_group(after_tail, dst_tail_size);
        }

        auto const src_base_lst = src_base_end + src.end().to_pointer().not_even();
        for (;src_base_pos != src_base_lst; ++src_base_pos) *dst_base_pos++ = shifter(*src_base_pos);

        if (dst_tail_size)
        {
            if (src_tail_size && (dst_tail_size >= src_tail_size))
            {
                regibits::apply_lsb_mask(*dst_base_end, dst_tail_size);
                *dst_base_end |= after_tail;
            }
            else
            {
                regibits::reset_lsb_group(*dst_base_end, dst_tail_size);
                byte rest2 = shifter.rest();
                regibits::apply_lsb_mask(rest2, dst_tail_size);
                *dst_base_end |= rest2;
            }
        }

        if (dst_offset)
        {
            dst_base_pos = dst.begin().to_pointer().base();
            regibits::reset_lsb_group(*dst_base_pos, dst_offset);
            *dst_base_pos |= before_prow;
        }
    }

}}}