#include <rubetek/config.hpp>
#ifndef RUBETEK_HEADER_ONLY
#   include <rubetek/essence/property.cpp>
#endif


namespace rubetek
{
    RUBETEK_INLINE_OF_IMPL property::property()
        : id()
    {}

    RUBETEK_INLINE_OF_IMPL property::property(id_type property_id, value_type property_value)
        : id    ( property_id    )
        , value ( property_value )
    {}
}
