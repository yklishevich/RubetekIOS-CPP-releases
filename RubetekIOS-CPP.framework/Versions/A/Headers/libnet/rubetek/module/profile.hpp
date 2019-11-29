#pragma once

#include <rubetek/module/context.hpp>
#include <rubetek/module/interface.hpp>
#include <rubetek/module/log/interface.hpp>


namespace rubetek {
namespace module  {

    struct profile
    {
        context_list             context             ;
        log::log_controller_link log_controller_link ;
        module_controller_link   controller_link     ;
    };

}}
