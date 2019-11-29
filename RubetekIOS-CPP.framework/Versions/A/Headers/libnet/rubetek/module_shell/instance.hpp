#pragma once

#include <functional>
#include <unordered_map>

#include <msw/assert.hpp>
#include <msw/std/memory.hpp>
#include <msw/make_range.hpp>
#include <msw/make_range_native.hpp>
#include <msw/throw_runtime_error.hpp>

#include <rubetek/module_shell/log.hpp>
#include <rubetek/module/interface.hpp>
#include <rubetek/module_shell/device.hpp>
#include <rubetek/module/log/interface.hpp>
#include <rubetek/essence/unique_id/editor.hpp>
#include <rubetek/essence/property/address.hpp>
#include <rubetek/module_shell/instance_back_interface.hpp>


namespace rubetek      {
namespace module_shell {

    template <typename Device = device>
    struct instance
        : rubetek::module::module_controller_interface
    {
        typedef Device device_type;

        explicit                                  instance                    (instance_back_interface* instance_back_interface)                         ;

        rubetek::module::log::log_controller_link log_link                    ()                                                                         ;
        rubetek::module::module_controller_link   back_link                   ()                                                                         ;
        void                                      init                        (rubetek::module::controller_module_link link)                             ;

        bool                                      accessible                  ()                                                                         ;
        void                                      process_inter_module_packet (msw::range<msw::byte const> packet)                                       ;
        void                                      set_project_settings        (msw::range<msw::byte const> settings)                                     ;
        void                                      command_from_editor         (msw::range<msw::byte const> id, msw::range<msw::byte const> packet)       ;
        bool                                      has_device                  (property_address device_id)                                         const ;
        bool                                      open_device                 (
                                                                                   property_address            device_id
                                                                                 , std::string const&          device_name
                                                                                 , msw::range<msw::byte const> block_settings
                                                                                 , msw::range<msw::byte const> block_property_map
                                                                                 , device::readable_property_id_list const&
                                                                               )                                                                         ;
        void                                      close_device                (property_address device_id)                                               ;
        void                                      change_properties           (property_address device_id, property_list const& properties)              ;
        void                                      system_command              (msw::range<msw::byte const> command)                                      ;
        void                                      idle                        ()                                                                         ;
        bool                                      has_link                    ()                                                                   const ;
        void                                      remove_devices              ()                                                                         ;

    private:

        typedef std::unique_ptr<device_type>                     device_ptr ;
        typedef std::unordered_map<property_address, device_ptr> devices    ;

        bool                                      add_device                  (
                                                                                   property_address            device_id
                                                                                 , std::string const&          device_name
                                                                                 , msw::range<msw::byte const> block_settings
                                                                                 , msw::range<msw::byte const> block_property_map
                                                                                 , device::readable_property_id_list const&
                                                                              )                                                                          ;
        void                                      remove_device               (device_ptr&)                                                              ;
        rubetek::module::controller_module_link   link                        ()                                                                         ;
        void                                      send_command                (utility::raw_memory const& id, utility::raw_memory const& mem)            ;
        void                                      reload_clients_list         ()                                                                         ;
        void                                      send_inter_module_packet    (utility::raw_memory const&)                                               ;

        instance_back_interface*                const back_interface_         ;
        bool                                          accessible_             ;
        module_log                                    module_log_             ;
        rubetek::module::controller_module_link       controller_module_link_ ;
        devices                                       devices_                ;
    };

}}


namespace rubetek      {
namespace module_shell {

    template <typename Device>
    instance<Device>::instance(instance_back_interface* instance_back_interface)
        : back_interface_ ( instance_back_interface )
        , accessible_     ( false                   )
        , module_log_
        (
            [this](rubetek::module::log::level level, std::string const& message)
            {
                back_interface_->instance_back_send_log(level, message);
            }
        )
        , controller_module_link_ (nullptr)
    {}

    template <typename Device>
    rubetek::module::log::log_controller_link instance<Device>::log_link()
    {
        return &module_log_;
    }
    template <typename Device>
    rubetek::module::module_controller_link instance<Device>::back_link()
    {
        return this;
    }
    template <typename Device>
    void instance<Device>::init(rubetek::module::controller_module_link link)
    {
        MSW_ASSERT(!controller_module_link_);
        controller_module_link_ = link;
    }

    template <typename Device>
    bool instance<Device>::accessible()
    {
        return link()->accessible();
    }
    template <typename Device>
    void instance<Device>::process_inter_module_packet(msw::range<msw::byte const> packet)
    {
        link()->process_inter_module_packet(utility::raw_memory(packet.data().native(), packet.size().count()));
    }
    template <typename Device>
    void instance<Device>::set_project_settings(msw::range<msw::byte const> settings)
    {
        link()->set_project_settings(utility::raw_memory(settings.data().native(), settings.size().count()));
    }
    template <typename Device>
    void instance<Device>::command_from_editor(msw::range<msw::byte const> id, msw::range<msw::byte const> packet)
    {
        link()->command_received(utility::raw_memory(id.data().native(), id.size().count()), utility::raw_memory(packet.data().native(), packet.size().count()));
    }
    template <typename Device>
    bool instance<Device>::has_device(property_address device_id) const
    {
        return devices_.find(device_id) != devices_.end();
    }
    template <typename Device>
    bool instance<Device>::open_device
    (
          property_address                         device_id
        , std::string const&                       device_name
        , msw::range<msw::byte const>              block_settings
        , msw::range<msw::byte const>              block_property_map
        , device::readable_property_id_list const& readable_property_id_list
    )
    {
        return add_device(device_id, device_name, block_settings, block_property_map, readable_property_id_list);
    }
    template <typename Device>
    void instance<Device>::close_device(property_address device_id)
    {
        auto it = devices_.find(device_id);
        MSW_ASSERT(it != devices_.end());
        remove_device(it->second);
        devices_.erase(it);
    }
    template <typename Device>
    void instance<Device>::change_properties(property_address device_id, property_list const& properties)
    {
        auto it = devices_.find(device_id);
        MSW_ASSERT(it != devices_.end());
        try
        {
            it->second->change_properties(properties);
        }
        catch(std::exception const& e)
        {
            msw::throw_runtime_error("change properties on device [", device_id, "]: ", e.what());
        }
    }
    template <typename Device>
    void instance<Device>::system_command(msw::range<msw::byte const> command)
    {
        link()->system_command(utility::raw_memory(command.data().native(), command.size().count()));
    }
    template <typename Device>
    void instance<Device>::idle()
    {
        if (has_link())
        {
            link()->idle();
            for (auto& pr : devices_)
            {
                pr.second->idle();
                property_list list = pr.second->check_readable_properties();
                if (!list.empty()) back_interface_->instance_back_send_properties(pr.first, list);
            }
            bool const accessible = link()->accessible();
            if (accessible != accessible_)
            {
                accessible_ = accessible;
                back_interface_->instance_back_send_accessible(accessible_);
            }
        }
    }
    template <typename Device>
    bool instance<Device>::has_link() const
    {
        return controller_module_link_ != nullptr;
    }
    template <typename Device>
    void instance<Device>::remove_devices()
    {
        for (auto& pr : devices_) remove_device(pr.second);
    }
    template <typename Device>
    bool instance<Device>::add_device
    (
          property_address                         device_id
        , std::string const&                       device_name
        , msw::range<msw::byte const>              block_settings
        , msw::range<msw::byte const>              block_property_map
        , device::readable_property_id_list const& readable_property_id_list
    )
    {
        device::session_ptr device_session_ptr(msw::make_unique<device_session>());
        rubetek::module::controller_module_interface::device_contex device_contex
        (
              device_name.c_str()
            , device_id.c_str()
            , utility::raw_memory(block_settings.data().native(), block_settings.size().count())
            , utility::raw_memory(block_property_map.data().native(), block_property_map.size().count())
            , device_session_ptr.get()
        );
        rubetek::module::controller_device_link device_link = link()->open_device(device_contex);
        if (device_link)
        {
            device_session_ptr->open(device_link);
            devices_.insert(std::make_pair(device_id, device_ptr(msw::make_unique<device_type>(readable_property_id_list, std::move(device_session_ptr)))));
            return true;
        }
        return false;
    }
    template <typename Device>
    void instance<Device>::remove_device(device_ptr& dev)
    {
        link()->close_device(dev->eject_session()->eject_link());
    }
    template <typename Device>
    rubetek::module::controller_module_link instance<Device>::link()
    {
        MSW_ASSERT(controller_module_link_);
        return controller_module_link_;
    }
    template <typename Device>
    void instance<Device>::send_command(utility::raw_memory const& id, utility::raw_memory const& mem)
    {
        back_interface_->instance_back_send_command(msw::make_range_native(id.data(), id.size()), msw::make_range_native(mem.data(), mem.size()));
    }
    template <typename Device>
    void instance<Device>::reload_clients_list()
    {
        back_interface_->instance_back_send_reload_clients_list();
    }
    template <typename Device>
    void instance<Device>::send_inter_module_packet(utility::raw_memory const& packet)
    {
        back_interface_->instance_back_send_inter_module_packet(msw::make_range_native(packet.data(), packet.size()));
    }

}}
