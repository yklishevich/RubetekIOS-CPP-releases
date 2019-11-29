#pragma once

#include <msw/size.hpp>
#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/dump/line.hpp>
#include <msw/dump/mem_info.hpp>


namespace msw
{
    template <typename Src>
    size<char> line_dump_mi_size(Src const& src)
    {
        return msw::mem_info_size(src) + 1 + msw::line_dump_size(src);
    }

    template <typename Src>
    void line_dump_mi(Src const& src, range<char> str, char splitter = ' ')
    {
        msw::mem_info(src, str.pop_front(msw::mem_info_size(src)));
        str.pop_front() = '\n';
        msw::line_dump(src, str, splitter);
    }

    template <typename Src>
    buffer<char> line_dump_mi(Src const& src, char splitter = ' ')
    {
        buffer<char> buf(msw::line_dump_mi_size(src) + 1);
        msw::line_dump_mi(src, buf.all(), splitter);
        buf.back() = 0;
        return buf;
    }
}
