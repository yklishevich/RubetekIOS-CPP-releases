#pragma once

#include <memory>

#include <rubetek/module/device/manager.hpp>
#include <rubetek/module/interface.hpp>
#include <rubetek/module/log/log.hpp>
#include <rubetek/module/property_map.hpp>
#include <rubetek/module/settings/settings.hpp>
#include <rubetek/module/context.hpp>
#include <rubetek/module/version.hpp>
#include <rubetek/module/context2.hpp>
#include <rubetek/module/command/content.hpp>


namespace rubetek {
namespace module  {

    template <typename Module>
    struct module_shell
        : controller_module_interface
    {
        typedef Module module_type;

                                module_shell  (context_list const&, module_controller_link) ;
                                ~module_shell ()                                            ;
        static  version_type    version       ()                                            ;

    private:

        bool                   accessible                  ()                                                                 ;
        void                   set_project_settings        (utility::raw_memory const&)                                       ;
        void                   command_received            (utility::raw_memory const& editor_id, utility::raw_memory const&) ;
        void                   system_command              (utility::raw_memory const&)                                       ;
        controller_device_link open_device                 (device_contex const& contex)                                      ;
        void                   close_device                (controller_device_link)                                           ;
        void                   idle                        ()                                                                 ;
        void                   process_inter_module_packet (utility::raw_memory const&)                                       ;

        context2::ptr                context_     ;
        std::unique_ptr<module_type> module_      ;
        device_list                  device_list_ ;
    };

}}


namespace rubetek {
namespace module  {

    template <typename Module>
    module_shell<Module>::module_shell(context_list const& context, module_controller_link link)
        : context_ ( context2::make(link)                                                         )
        , module_  ( new module_type(settings(unpack<settings_list>(context.settings)), context_) )
    {}
    template <typename Module>
    module_shell<Module>::~module_shell()
    {
        module_.reset();
        context_->free();
    }

    template <typename Module>
    version_type module_shell<Module>::version()
    {
        return module_type::version();
    }

    template <typename Module>
    bool module_shell<Module>::accessible()
    {
        return module_->accessible();
    }

    template <typename Module>
    void module_shell<Module>::set_project_settings(utility::raw_memory const& mem)
    {
        module_->set_project_settings(settings(unpack<settings_list>(mem)));
    }

    template <typename Module>
    void module_shell<Module>::command_received(utility::raw_memory const& editor_id, utility::raw_memory const& command)
    {
        module_->command_received(unique_editor_id_type(editor_id.data(), editor_id.data() + editor_id.size()), command.data(), command.size());
    }

    template <typename Module>
    void module_shell<Module>::system_command(utility::raw_memory const& command)
    {
        module_->system_command(command.data(), command.size());
    }

    template <typename Module>
    controller_device_link module_shell<Module>::open_device(device_contex const& contex)
    {
        controller_device_link link;
        try
        {
            device_ptr device_ptr_;
            settings settings_(unpack<settings_list>(contex.settings), contex.id);
            module_->open_device
            (
                contex.name, device_manager
                (
                      device_ptr_
                    , settings_
                    , unpack<property_map>(contex.property_map)
                    , std::bind(&module_shell<module_type>::accessible, this)
                    , contex.controller_link
                )
            );
            link = device_ptr_.get();
            if (device_ptr_) device_list_.push_back(std::move(device_ptr_));
            else throw std::runtime_error("device not found in module implementation");
        }
        catch (std::exception& e)
        {
            link = 0;
            log::error("open device '" + std::string(contex.name) + "': " + e.what());
        }
        return link;
    }
    template <typename Module>
    void module_shell<Module>::close_device(controller_device_link link)
    {
        device_list_.remove_if
        (
            [link](device_ptr const& d)
            {
                return d.get() == link;
            }
        );
    }

    template <typename Module>
    void module_shell<Module>::idle()
    {
        module_->idle();
    }

    template <typename Module>
    void module_shell<Module>::process_inter_module_packet(utility::raw_memory const& pkt)
    {
        context_->process_packet(msw::make_range_native(pkt.data(), pkt.size()));
    }

}}
