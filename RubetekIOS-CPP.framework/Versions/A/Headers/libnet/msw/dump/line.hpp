#pragma once

#include <msw/size.hpp>
#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/make_range.hpp>
#include <msw/zzz/dump/line.hpp>


namespace msw
{
    template <typename Src>
    size<char> line_dump_size(Src const& src)
    {
        return zzz::line_dump_size(make_range(src));
    }

    template <typename Src>
    void line_dump(Src const& src, range<char> str, char splitter = ' ')
    {
        zzz::line_dump(make_range(src), str, splitter);
    }

    template <typename Src>
    buffer<char> line_dump(Src const& src, char splitter = ' ')
    {
        buffer<char> buf(msw::line_dump_size(src) + 1);
        msw::line_dump(src, buf.all(), splitter);
        buf.back() = 0;
        return buf;
    }
}
