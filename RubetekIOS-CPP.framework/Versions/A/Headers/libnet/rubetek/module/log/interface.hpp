#pragma once

#include <rubetek/module/log/level.hpp>
#include <rubetek/module/noncopyable.hpp>


namespace rubetek {
namespace module  {
namespace log     {

    struct log_controller_interface
        : noncopyable
    {
        virtual void write(char const*, level) const = 0;
    };
    typedef log_controller_interface* log_controller_link;

}}}
