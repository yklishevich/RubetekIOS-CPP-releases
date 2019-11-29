#pragma once

#include <list>
#include <string>

#include <rubetek/utility/multiform/multiform.hpp>


namespace rubetek {
namespace module  {

    struct setting
    {
        typedef multiform value_type;

        std::string name  ;
        value_type  value ;

        setting();
        setting(std::string const& name, value_type const&);
    };

    typedef std::list<setting> settings_list;

}}


namespace rubetek {
namespace module  {

    inline setting::setting()
        : name  ()
        , value ()
    {}
    inline setting::setting(std::string const& name, value_type const& value)
        : name  ( name  )
        , value ( value )
    {}

}}
