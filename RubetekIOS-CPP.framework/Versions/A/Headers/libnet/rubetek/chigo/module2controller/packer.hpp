#pragma once

#include <msw/range.hpp>

#include <rubetek/config.hpp>
#include <rubetek/chigo/packer.hpp>


namespace chigo             {
namespace module2controller {

    struct packer
        : chigo::packer
    {
        explicit packer(msw::range<msw::byte>);
    };

}}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/chigo/module2controller/impl/packer.cpp>
#endif
