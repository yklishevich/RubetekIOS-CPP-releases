#pragma once

#include <vector>
#include <memory>

#include <rubetek/essence/property/id.hpp>
#include <rubetek/utility/multiform/multiform.hpp>


namespace rubetek {
namespace module  {

    struct property_accessor
    {
        typedef multiform value_type;

        virtual value_type read  ()                  = 0;
        virtual void       write (value_type const&) = 0;

        virtual ~property_accessor() {}
    };
    typedef std::unique_ptr<property_accessor>                property_accessor_ptr ;
    typedef std::map<property_id_type, property_accessor_ptr> property_accessor_map ;

}}
