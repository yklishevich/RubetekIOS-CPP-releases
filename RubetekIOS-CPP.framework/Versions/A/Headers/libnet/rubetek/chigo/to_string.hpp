#pragma once

#include <string>

#include <rubetek/config.hpp>
#include <rubetek/chigo/controls.hpp>


namespace chigo
{
    std::string to_string( electric_control );
    std::string to_string( speeds_control   );
    std::string to_string( damper_control   );
    std::string to_string( mode_control     );
}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/chigo/impl/to_string.cpp>
#endif
