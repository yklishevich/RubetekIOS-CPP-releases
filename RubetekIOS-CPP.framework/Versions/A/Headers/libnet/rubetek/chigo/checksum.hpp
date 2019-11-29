#pragma once

#include <msw/range.hpp>

#include <rubetek/config.hpp>


namespace chigo
{
    msw::byte packet_checksum(msw::range<msw::byte const>);
}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/chigo/impl/checksum.cpp>
#endif
