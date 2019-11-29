#pragma once

#include <vector>
#include <string>

#ifndef RUBETEK_CONTROLLER_LINK_LIBRARY
#   include <lama/boost/include/variant.hpp>
#else
#   include <boost/variant.hpp>
#endif

#ifndef RUBETEK_CONTROLLER_LINK_LIBRARY
#   include <rubetek/config.hpp>
#endif
#include <rubetek/essence/property/id.hpp>
#include <rubetek/essence/property/address.hpp>


namespace rubetek
{
    struct property
    {
        typedef property_id_type                              id_type    ;
        typedef boost::variant<bool, int, float, std::string> value_type ;

        id_type    id    ;
        value_type value ;

        property();
        property(id_type id, value_type);
    };

    typedef std::vector<property> property_list;
}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/essence/impl/property.cpp>
#endif
