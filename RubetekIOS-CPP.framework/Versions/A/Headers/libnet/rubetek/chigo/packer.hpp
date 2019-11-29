#pragma once

#include <msw/range.hpp>

#include <rubetek/config.hpp>
#include <rubetek/chigo/controls.hpp>


namespace chigo
{
    struct packer
    {
        explicit packer(msw::range<msw::byte>, int offset = 0);

        void receiver_address               (int)              ;
        void transmitter_address            (int)              ;
        void aided_electric_heating_control (electric_control) ;
        void economic                       (bool)             ;
        void turbo                          (bool)             ;
        void sleep                          (bool)             ;
        void control_on_off                 (bool)             ;
        void health                         (bool)             ;
        void clean                          (bool)             ;
        void anion                          (bool)             ;
        void wind_speeds_control            (speeds_control)   ;
        void indoor_air_damper_control      (bool)             ;
        void outdoor_air_damper_control     (damper_control)   ;
        void mode                           (mode_control)     ;
        void temperature                    (int)              ;
        void timing                         (bool)             ;
        void timing_on                      (bool)             ;
        void timing_off                     (bool)             ;
        void time_of_timing                 (int)              ;
        void checksum                       ()                 ;

    protected:
        msw::range<msw::byte> pkt_;
    private:
        int offset_;
    };
}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/chigo/impl/packer.cpp>
#endif
