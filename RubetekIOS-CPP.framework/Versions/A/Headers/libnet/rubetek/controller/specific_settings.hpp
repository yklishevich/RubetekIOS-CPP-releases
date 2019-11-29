#pragma once

#include <list>
#include <string>
#include <utility>


namespace rubetek    {
namespace controller {

    typedef std::pair<std::string/*name*/, std::string/*value*/> specific_setting  ;
    typedef std::list<specific_setting>                          specific_settings ;

}}
