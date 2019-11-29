#pragma once

#include <string>

#include <msw/make_range.hpp>
#include <msw/zzz/dump/custom.hpp>


namespace msw
{
    template <typename Src>
    std::string custom_dump(Src const& src)
    {
        return zzz::custom_dump(make_range(src));
    }
}
