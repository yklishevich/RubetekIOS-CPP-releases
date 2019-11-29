#pragma once

#include <msw/range.hpp>

#include <rubetek/config.hpp>
#include <rubetek/chigo/unpacker.hpp>


namespace chigo             {
namespace module2controller {

    struct unpacker
        : chigo::unpacker
    {
        explicit unpacker(msw::range<msw::byte const>);
    };

}}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/chigo/module2controller/impl/unpacker.cpp>
#endif
