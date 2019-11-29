#pragma once

#include <msw/range.hpp>

#include <rubetek/config.hpp>
#include <rubetek/chigo/packer.hpp>


namespace chigo             {
namespace controller2module {

    struct packer
        : chigo::packer
    {
        explicit packer(msw::range<msw::byte>);

        void indoor_pipe_temp  (bool) ;
        void indoor_room_temp  (bool) ;
        void outdoor_pipe_temp (bool) ;
    };

}}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/chigo/controller2module/impl/packer.cpp>
#endif
