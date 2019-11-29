#pragma once

#include <rubetek/module/noncopyable.hpp>
#include <rubetek/utility/raw_memory.hpp>
#include <rubetek/essence/property/id.hpp>


namespace rubetek {
namespace module  {

    struct controller_device_interface
        : noncopyable
    {
        virtual bool accessible          ()                                      = 0;
        virtual void write_property      (property_id_type, utility::raw_memory) = 0;
        virtual void read_property_value (property_id_type)                      = 0;
        virtual void idle                ()                                      = 0;
    };
    typedef controller_device_interface* controller_device_link;
    struct device_controller_interface
        : noncopyable
    {
        virtual void property_value_readed (utility::raw_memory) = 0;
    };
    typedef device_controller_interface* device_controller_link;

}}
