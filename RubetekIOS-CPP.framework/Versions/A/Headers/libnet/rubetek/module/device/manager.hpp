#pragma once

#include <unordered_map>

#include <rubetek/utility/noncopyable.hpp>
#include <rubetek/module/device/device.hpp>
#include <rubetek/module/device/shell.hpp>
#include <rubetek/module/property_map.hpp>
#include <rubetek/module/settings/settings.hpp>
#include <rubetek/module/accessible_type.hpp>


namespace rubetek {
namespace module  {

    struct device_manager
        : utility::noncopyable
    {
        device_manager(device_ptr&, settings const&, property_map const&, accessible_type, device_controller_link);
        device_manager(device_manager&&);

        template <typename Device, typename ...Args>
        void open(Args&&...);

    private:
        device_ptr&            device_ptr_             ;
        settings               settings_               ;
        property_map           property_map_           ;
        accessible_type        module_accessible_      ;
        device_controller_link device_controller_link_ ;
    };

}}


namespace rubetek {
namespace module  {

    inline device_manager::device_manager
    (
          device_ptr&            device_ptr
        , settings const&        settings
        , property_map const&    property_map
        , accessible_type        module_accessible
        , device_controller_link device_controller_link
    )
        : device_ptr_             ( device_ptr             )
        , settings_               ( settings               )
        , property_map_           ( property_map           )
        , module_accessible_      ( module_accessible      )
        , device_controller_link_ ( device_controller_link )
    {}
    inline device_manager::device_manager(device_manager&& rhs)
        : device_ptr_             ( rhs.device_ptr_                   )
        , settings_               ( std::move(rhs.settings_)          )
        , property_map_           ( std::move(rhs.property_map_)      )
        , module_accessible_      ( std::move(rhs.module_accessible_) )
        , device_controller_link_ ( rhs.device_controller_link_       )
    {}

    template <typename Device, typename ...Args>
    void device_manager::open(Args&&... args)
    {
        device_ptr_.reset(new device_shell<Device>(module_accessible_, device_controller_link_, settings_, property_map_, std::forward<Args>(args)...));
    }

}}
