#pragma once

#include <string>

#ifndef RUBETEK_CONTROLLER_LINK_LIBRARY
#   include <rubetek/config.hpp>
#endif
#include <rubetek/essence/property.hpp>


namespace rubetek
{
    std::string property_to_string      ( property const&      ) ;
    std::string property_list_to_string ( property_list const& ) ;
}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/essence/impl/property_string_cast.cpp>
#endif
