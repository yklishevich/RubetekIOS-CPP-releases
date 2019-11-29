#pragma once

#include <list>
#include <memory>

#include <rubetek/module/device/interface.hpp>


namespace rubetek {
namespace module  {

    struct device
        : controller_device_interface
    {
        virtual ~device() {}
    };

    typedef std::unique_ptr<device> device_ptr  ;
    typedef std::list<device_ptr>   device_list ;

}}
