#pragma once

#include <list>

#include <rubetek/essence/property.hpp>


namespace rubetek
{
    struct device_properties
    {
        property_address address    ;
        property_list    properties ;
    };

    typedef std::list<device_properties> device_properties_list;
}
