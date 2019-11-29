#pragma once

#include <string>

#include <msw/dump/custom.hpp>
#include <msw/dump/mem_info.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>


namespace msw
{
    template <typename Src>
    std::string custom_dump_mi(Src const& src)
    {
        return bytes_as_string(msw::mem_info(src)) + "\n" + msw::custom_dump(src);
    }
}
