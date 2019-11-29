#pragma once

#include <msw/range.hpp>

#include <rubetek/config.hpp>
#include <rubetek/chigo/controls.hpp>


namespace chigo
{
    struct unpacker
    {
        explicit unpacker(msw::range<msw::byte const>, int offset = 0);

        bool             check_preamble_code            () const ;
        bool             check_version                  () const ;
        bool             check_checksum                 () const ;

        int              receiver_address               () const ;
        int              transmitter_address            () const ;
        electric_control aided_electric_heating_control () const ;
        bool             economic                       () const ;
        bool             turbo                          () const ;
        bool             sleep                          () const ;
        bool             control_on_off                 () const ;
        bool             health                         () const ;
        bool             clean                          () const ;
        bool             anion                          () const ;
        speeds_control   wind_speeds_control            () const ;
        bool             indoor_air_damper_control      () const ;
        damper_control   outdoor_air_damper_control     () const ;
        mode_control     mode                           () const ;
        int              temperature                    () const ;
        bool             timing                         () const ;
        bool             timing_on                      () const ;
        bool             timing_off                     () const ;
        int              time_of_timing                 () const ;

    protected:
        msw::range<msw::byte const> pkt_;
    private:
        int offset_;
    };
}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/chigo/impl/unpacker.cpp>
#endif
