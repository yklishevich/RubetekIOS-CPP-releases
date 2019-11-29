#pragma once

#include <list>
#include <string>

#ifndef RUBETEK_CONTROLLER_LINK_LIBRARY
#   include <msw/range.hpp>
#endif

#include <rubetek/config.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/essence/unique_id/client.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/account_projects.hpp>
#include <rubetek/client/controller_link_context.hpp>
#include <rubetek/client/controller_link_back_interface.hpp>


namespace rubetek {
namespace client  {

    struct controller_link_impl;

    struct controller_link
    {
             controller_link          (
                                            controller_link_back_interface*
                                          , unique_client_id::const_reference
                                          , controller_link_context const&
                                          , bool                              focus                 = true
                                          , bool                              disable_start_logic   = false
                                          , bool                              disable_find_on_empty = true
                                      )                                                               ;
             ~controller_link         ()                                                              ;

        void auto_connect_by_location (bool yes)                                                      ;
        bool auto_connect_by_location ()                                                        const ;
        void auto_connect_to_recent   (bool yes)                                                      ;
        bool auto_connect_to_recent   ()                                                        const ;

        void focus                    (bool yes)                                                      ;

        void show_unknown_controllers ()                                                              ;
        void show_known_controllers   ()                                                              ;
        void show_all_controllers     ()                                                              ;
        void cancel_show_list         ()                                                              ;
        void cancel_find_controller   ()                                                              ;

        void rename_controller        (unique_controller_id::const_reference, std::string name)       ;
        void remove_controller        (unique_controller_id::const_reference)                         ;
        void remove_all_controllers   ()                                                              ;

        void connect_to               (unique_controller_id::const_reference)                         ;
        void disconnect               ()                                                              ;

        int  updating_progress        ()                                                        const ;

        void refresh                  ()                                                              ;

        void send_password            (std::string)                                                   ;
        void send_project             (std::string const&)                                            ;
        void send_property            (property_address, property)                                    ;
        void send_property            (property_address, property_list)                               ;
#ifndef RUBETEK_CONTROLLER_LINK_LIBRARY
        void send_command             (msw::range<msw::byte const>)                                   ;
        void send_command             (msw::byte const*, msw::uint size)                              ;
#else
        void send_command             (unsigned char const*, unsigned size)                           ;
#endif
        void send_command             (std::string const&)                                            ;
        void send_server_directly     (server_packet_type, std::string const&)                        ;
        void send_server_auxiliary    (std::string const&)                                            ;
        void send_notification_id     (std::string const&)                                            ;

        void remove_client            (unique_controller_id::const_reference)                         ;

        void synchronize_projects     (account_projects const&)                                       ;

        void process_events           ()                                                              ;

        static std::list<unique_controller_id::type>
             projects_id_list         (std::string const& work_path)                                  ;

    private:
                         controller_link (controller_link const&) = delete ;
        controller_link& operator =      (controller_link const&) = delete ;

        controller_link_impl* impl_;
    };

}}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/impl/controller_link.cpp>
#endif
