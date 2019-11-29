#pragma once

#include <string>

#include <rubetek/config.hpp>
#include <rubetek/chigo/controls.hpp>


namespace chigo
{
    electric_control electric_from_string (std::string) ;
    speeds_control   speeds_from_string   (std::string) ;
    damper_control   damper_from_string   (std::string) ;
    mode_control     mode_from_string     (std::string) ;
}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/chigo/impl/from_string.cpp>
#endif
