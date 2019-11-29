#pragma once

#include <rubetek/controller/specific_settings.hpp>


namespace rubetek    {
namespace controller {

    typedef std::pair<std::string/*module name*/, specific_settings> module_specific_settings  ;
    typedef std::list<module_specific_settings>                      modules_specific_settings ;

}}
