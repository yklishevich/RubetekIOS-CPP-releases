#pragma once

#include <memory>

#include <msw/config.hpp>
#include <msw/range.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/callback/event.hpp>
#include <msw/percentage_relation.hpp>

#include <rubetek/log.hpp>
#include <rubetek/config.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/local_link/interface.hpp>
#include <rubetek/remote_link/interface.hpp>
#include <rubetek/client/controller_info.hpp>
#include <rubetek/essence/unique_id/channel.hpp>
#include <rubetek/proto/ihmlp/async_transceiver.hpp>
#include <rubetek/client/controller/ihap_transceiver.hpp>
#include <rubetek/client/controller/controller_back_interface.hpp>
#include <rubetek/client/controller/active_controller_back_interface.hpp>


namespace rubetek {
namespace client  {

    struct controller
        : ihap_transceiver_callback
    {
        enum class state
        {
              offline
            , init
            , standby
            , authentication
            , loading_project
            , active
        };

        explicit                              controller           (
                                                                         unique_controller_id::const_reference
                                                                       , controller_back_interface*
                                                                       , bool                                  known = false
                                                                   )                                                               ;

        void                                  add_local_link       (
                                                                         local_link_interface*
                                                                       , bool                  known
                                                                   )                                                               ;
        void                                  remove_local_link    ()                                                              ;
        void                                  add_remote_link      (
                                                                         remote_link_interface<>*
                                                                       , std::string const& ip_address
                                                                       , unique_channel_id_type const&
                                                                   )                                                               ;
        void                                  remove_remote_link   ()                                                              ;

        unique_controller_id::const_reference id                   ()                                                        const ;
        std::string                           name                 ()                                                        const ;
        state                                 get_state            ()                                                        const ;
        bool                                  known                ()                                                        const ;
        bool                                  online               ()                                                        const ;
        bool                                  active               ()                                                        const ;
        bool                                  can_connect          ()                                                        const ;
        std::string                           ip_address           ()                                                        const ;
        std::string                           local_ip_address     ()                                                        const ;
        std::string                           remote_ip_address    ()                                                        const ;
        bool                                  has_info             ()                                                        const ;
        controller_info                       make_info            ()                                                        const ;
        bool                                  has_local_link       ()                                                        const ;
        bool                                  demo_mode            ()                                                        const ;
        controller_availability               availability         ()                                                        const ;

        void                                  connect              (
                                                                         active_controller_back_interface*
                                                                       , device_info const&
                                                                       , unsigned           project_crc
                                                                       , std::string const& project_alias
                                                                   )                                                               ;
        void                                  disconnect           ()                                                              ;

        void                                  send_password        (std::string const&)                                            ;
        void                                  send_project         (std::string const&)                                            ;
        void                                  send_property        (property_address, property_list const&)                        ;
        void                                  send_module_command  (msw::range<msw::byte const>)                                   ;
        void                                  send_project_alias   (std::string const&)                                            ;
        void                                  send_notification_id (std::string const&)                                            ;
        void                                  refresh              ()                                                              ;

        void                                  set_auto_known       ()                                                              ;
        void                                  make_unknown         ()                                                              ;

        void                                  remove_client        (unique_client_id::const_reference)                             ;

        static std::string                    state_to_str         (state)                                                         ;

    private:

        struct info
        {
            std::string name           ;
            std::string version        ;
            std::string system_info    ;
            bool        demo_mode      ;
            std::string server_address ;

            info
            (
                  std::string name
                , std::string version
                , std::string system_info
                , bool        demo_mode
                , std::string server_address
            )
                : name           ( name           )
                , version        ( version        )
                , system_info    ( system_info    )
                , demo_mode      ( demo_mode      )
                , server_address ( server_address )
            {}
        };

        struct ip_address_pair
        {
            std::function<std::string()> get_local ;
            std::string                  remote    ;
        };

        struct wait_project
        {
            unsigned    crc ;
            enum
            {
                project
            }           type;
        };

        struct project_loader
        {
            unsigned                      complete_size ;
            unsigned                      loaded_size   ;
            std::unique_ptr<wait_project> wait_prj      ;

            project_loader(unsigned size)
                : complete_size ( size )
                , loaded_size   ( 0    )
            {}
        };

        void on_controller_info      (
                                           std::string const& name
                                         , std::string const& version
                                         , std::string const& system_info
                                         , bool               demo_mode
                                         , std::string const& server_address
                                     )                                        final ;
        void on_controller_rename    (std::string const&)                     final ;
        void on_remove               ()                                       final ;
        void on_query_password       (option_was_removed)                     final ;
        void on_no_project           ()                                       final ;
        void on_update_project       (unsigned size)                          final ;
        void on_large_block          (msw::range<msw::byte const>)            final ;
        void on_large_block_enriched (msw::size<msw::byte>)                   final ;
        void on_loading_cancel       ()                                       final ;
        void on_project              (unsigned crc)                           final ;
        void on_ready                ()                                       final ;
        void on_property             (property_address, property_list const&) final ;
        void on_module_command       (std::string const&)                     final ;

        active_controller_back_interface*
             active_back_interface   ()                                       const ;

        void set_state               (state)                                        ;
        void set_offline             ()                                             ;
        void set_known               ()                                             ;
        void start_init              ()                                             ;

        logger                            const logger_                ;
        unique_controller_id::type        const id_                    ;
        controller_back_interface*        const back_interface_        ;
        state                                   state_                 ;
        msw::trigger                            known_                 ;
        msw::trigger                            auto_known_            ;
        ihap_transceiver                        ihap_transceiver_      ;
        ihmlp::async_transceiver                ihmlp_transceiver_     ;
        active_controller_back_interface*       active_back_interface_ ;
        ip_address_pair                         ip_address_            ;
        unique_channel_id_type                  remote_channel_id_     ;
        std::unique_ptr<info>                   info_                  ;
        std::unique_ptr<project_loader>         project_loader_        ;
    };

}}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/controller/impl/controller.cpp>
#endif
