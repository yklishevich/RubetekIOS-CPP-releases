#pragma once

#include <msw/size.hpp>
#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/make_range.hpp>
#include <msw/zzz/dump/dense.hpp>


namespace msw
{
    template <typename Src>
    size<char> dense_dump_size(Src const& src)
    {
        return zzz::dense_dump_size(make_range(src));
    }

    template <typename Src>
    void dense_dump(Src const& src, range<char> str)
    {
        zzz::dense_dump(make_range(src), str);
    }

    template <typename Src>
    buffer<char> dense_dump(Src const& src)
    {
        buffer<char> buf(msw::dense_dump_size(src) + 1);
        msw::dense_dump(src, buf.all());
        buf.back() = 0;
        return buf;
    }
}
