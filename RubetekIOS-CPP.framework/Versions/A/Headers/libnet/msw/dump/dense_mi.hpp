#pragma once

#include <msw/size.hpp>
#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/dump/dense.hpp>
#include <msw/dump/mem_info.hpp>


namespace msw
{
    template <typename Src>
    size<char> dense_dump_mi_size(Src const& src)
    {
        return msw::mem_info_size(src) + 1 + msw::dense_dump_size(src);
    }

    template <typename Src>
    void dense_dump_mi(Src const& src, range<char> str)
    {
        msw::mem_info(src, str.pop_front(msw::mem_info_size(src)));
        str.pop_front() = '\n';
        msw::dense_dump(src, str);
    }

    template <typename Src>
    buffer<char> dense_dump_mi(Src const& src)
    {
        buffer<char> buf(msw::dense_dump_mi_size(src) + 1);
        msw::dense_dump_mi(src, buf.all());
        buf.back() = 0;
        return buf;
    }
}
