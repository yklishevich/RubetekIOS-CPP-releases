#pragma once

#include <string>
#include <vector>

#include <rubetek/essence/property.hpp>

#include "property_types.h"


namespace rubetek
{
    proto::imc::property_value        property_value_to_thrift         (property::value_type const&)              ;
    proto::imc::property              property_to_thrift               (property const&)                          ;
    std::vector<proto::imc::property> property_list_to_thrift          (property_list const&)                     ;

    property::value_type              property_value_from_thrift       (proto::imc::property_value const&)        ;
    property                          property_from_thrift             (proto::imc::property const&)              ;
    property_list                     property_list_from_thrift        (std::vector<proto::imc::property> const&) ;

    proto::imc::property_type::type   thrift_property_type_from_string (std::string const&)                       ;
}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/essence/impl/property_thrift_cast.cpp>
#endif
