#pragma once

#include <msw/range.hpp>

#include <rubetek/config.hpp>
#include <rubetek/chigo/unpacker.hpp>


namespace chigo             {
namespace controller2module {

    struct unpacker
        : chigo::unpacker
    {
        explicit unpacker(msw::range<msw::byte const>);

        bool indoor_pipe_temp  () const ;
        bool indoor_room_temp  () const ;
        bool outdoor_pipe_temp () const ;
    };

}}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/chigo/controller2module/impl/unpacker.cpp>
#endif
