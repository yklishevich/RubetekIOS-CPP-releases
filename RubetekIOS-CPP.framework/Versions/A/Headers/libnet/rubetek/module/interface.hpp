#pragma once

#include <rubetek/module/noncopyable.hpp>
#include <rubetek/utility/raw_memory.hpp>
#include <rubetek/module/device/interface.hpp>
#include <rubetek/module/version.hpp>
#include <rubetek/essence/unique_id/editor.hpp>


namespace rubetek {
namespace module  {

    struct controller_module_interface
        : noncopyable
    {
        struct device_contex
        {

            char const*            name            ;
            char const*            id              ;
            utility::raw_memory    settings        ;
            utility::raw_memory    property_map    ;
            device_controller_link controller_link ;

            device_contex
            (
                  char const*                name
                , char const*                id
                , utility::raw_memory const& settings
                , utility::raw_memory const& property_map
                , device_controller_link
            );
        };

        virtual bool                   accessible                  ()                                                                 = 0 ;
        virtual void                   set_project_settings        (utility::raw_memory const&)                                       = 0 ;
        virtual void                   command_received            (utility::raw_memory const& editor_id, utility::raw_memory const&) = 0 ;
        virtual void                   system_command              (utility::raw_memory const&)                                       = 0 ;
        virtual controller_device_link open_device                 (device_contex const&)                                             = 0 ;
        virtual void                   close_device                (controller_device_link)                                           = 0 ;
        virtual void                   idle                        ()                                                                 = 0 ;
        virtual void                   process_inter_module_packet (utility::raw_memory const&)                                       = 0 ;
    };
    typedef controller_module_interface* controller_module_link;

    struct module_controller_interface
        : noncopyable
    {
        virtual void send_command             (utility::raw_memory const& editor_id, utility::raw_memory const&) = 0 ;
        virtual void reload_clients_list      ()                                                                 = 0 ;
        virtual void send_inter_module_packet (utility::raw_memory const&)                                       = 0 ;
    };
    typedef module_controller_interface* module_controller_link;

}}


namespace rubetek {
namespace module  {

    inline controller_module_interface::device_contex::device_contex
    (
          char const*                name
        , char const*                id
        , utility::raw_memory const& settings
        , utility::raw_memory const& property_map
        , device_controller_link     controller_link
    )
        : name            ( name            )
        , id              ( id              )
        , settings        ( settings        )
        , property_map    ( property_map    )
        , controller_link ( controller_link )
    {}
}}
