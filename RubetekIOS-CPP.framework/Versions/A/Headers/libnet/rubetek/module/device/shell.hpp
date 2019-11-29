#pragma once

#include <memory>

#include <rubetek/module/accessible_type.hpp>
#include <rubetek/module/settings/settings.hpp>
#include <rubetek/module/device/property/manager.hpp>
#include <rubetek/utility/ctti.hpp>
#include <rubetek/utility/package_std.hpp>
#include <rubetek/utility/multiform/package.hpp>


namespace rubetek {
namespace module  {

    template <typename Device>
    struct device_shell
        : device
    {
        typedef Device device_type;

        template <typename ...Args>
             device_shell        (accessible_type, device_controller_link, settings const&, property_map const&, Args&&...) ;

    private:
        void write_property      (property_id_type id, utility::raw_memory raw_memory)                                      ;
        void read_property_value (property_id_type id)                                                                      ;

        bool accessible          ()                                                                                         ;
        void idle                ()                                                                                         ;

        accessible_type              module_accessible_      ;
        device_controller_link       device_controller_link_ ;
        property_accessor_map        property_accessors_     ;
        std::unique_ptr<device_type> device_                 ;
    };

}}


namespace rubetek {
namespace module  {

    inline void check_property_accessors(property_accessor_map const& accessors, property_map const& property_map)
    {
        for (auto const& prop : property_map)
            if (accessors.find(prop.first) == accessors.end())
                throw std::runtime_error("property '" + prop.second + "'' with index " + std::to_string(prop.first) + " is not set");
        for (auto const& a : accessors)
            if (property_map.find(a.first) == property_map.end())
                throw std::runtime_error("installed property with index " + std::to_string(a.first) + " is unknown");
    }

    template <typename Device>
    template <typename ...Args>
    device_shell<Device>::device_shell
    (
          accessible_type        module_accessible
        , device_controller_link device_controller_link
        , settings const&        settings
        , property_map const&    property_map
        , Args&&...              args
    )
        : module_accessible_      ( module_accessible                                                                                           )
        , device_controller_link_ ( device_controller_link                                                                                      )
        , device_                 ( new device_type(settings, property_manager(property_accessors_, property_map), std::forward<Args>(args)...) )
    {
        check_property_accessors(property_accessors_, property_map);
    }

    template <typename Device>
    void device_shell<Device>::write_property(property_id_type id, utility::raw_memory raw_memory)
    {
        if (property_accessors_.find(id) == property_accessors_.end()) throw std::runtime_error("write property fail. bad id: " + std::to_string(id));
        property_accessors_[id]->write(unpack<multiform>(buffer(raw_memory.begin(), raw_memory.end())));
    }
    template <typename Device>
    void device_shell<Device>::read_property_value(property_id_type id)
    {
        if (id && (property_accessors_.find(id) == property_accessors_.end())) throw std::runtime_error("read property fail. bad id: " + std::to_string(id));
        multiform value = id ? property_accessors_[id]->read() : accessible();
        buffer buf(pack(value));
        assert(device_controller_link_);
        device_controller_link_->property_value_readed(utility::raw_memory(buf));
    }

    template <typename Device>
    bool device_shell<Device>::accessible()
    {
        return module_accessible_() && device_->accessible();
    }
    template <typename Device>
    void device_shell<Device>::idle()
    {
        device_->idle();
    }

}}
