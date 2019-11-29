#pragma once

#include <msw/size.hpp>
#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/make_range.hpp>
#include <msw/zzz/dump/mem_info.hpp>


namespace msw
{
    template <typename Src>
    size<char> mem_info_size(Src const& src)
    {
        return zzz::mem_info_size(make_range(src));
    }

    template <typename Src>
    position<char> mem_info(Src const& src, range<char> str)
    {
        return zzz::mem_info(make_range(src), str);
    }

    template <typename Src>
    buffer<char> mem_info(Src const& src)
    {
        buffer<char> buf(msw::mem_info_size(src) + 1);
        buf.erase_from_to_back(msw::mem_info(src, buf.all()));
        buf.push_back_silent(char(0));
        return buf;
    }
}
