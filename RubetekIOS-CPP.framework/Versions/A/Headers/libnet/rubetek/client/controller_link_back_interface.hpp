#pragma once

#include <string>

#include <rubetek/essence/property.hpp>
#include <rubetek/client/was_removed.hpp>
#include <rubetek/client/project_error.hpp>
#include <rubetek/client/controller_info.hpp>
#include <rubetek/client/controller_error.hpp>
#include <rubetek/client/server_connection_status.hpp>
#include <rubetek/client/remote/server_packet_type.hpp>


namespace rubetek {
namespace client  {

    struct controller_link_back_interface
    {
        virtual void on_find_controller              ()                                               = 0 ;
        virtual void on_unknown_controllers          (unknown_controller_list)                        = 0 ;
        virtual void on_known_controllers            (known_controller_list)                          = 0 ;
        virtual void on_all_controllers              (known_controller_list, unknown_controller_list) = 0 ;
        virtual void on_notify_about_location        (known_controller_list)                          = 0 ;
        virtual void on_server_connection_status     (server_connection_status)                       = 0 ;
        virtual void on_connecting_to_controller     (controller_info, std::string alias)             = 0 ;
        virtual void on_controller_connection_status (controller_availability, std::string ip)        = 0 ;
        virtual void on_controller_rename            (std::string const&)                             = 0 ;
        virtual void on_controller_error             (controller_error)                               = 0 ;
        virtual void on_password_request             (option_was_removed)                             = 0 ;
        virtual void on_project_error                (project_error)                                  = 0 ;
        virtual void on_project_updating             ()                                               = 0 ;
        virtual void on_ready                        (std::string project_file)                       = 0 ;
        virtual void on_properties_received          (property_address, property_list)                = 0 ;
        virtual void on_module_command               (std::string const&)                             = 0 ;
        virtual void on_server_directly              (server_packet_type, std::string const&)         = 0 ;
        virtual void on_server_auxiliary             (std::string const&)                             = 0 ;
        virtual void on_update_required              ()                                               = 0 ;
    protected:
         controller_link_back_interface() {}
        ~controller_link_back_interface() {}
    private:
                                        controller_link_back_interface (controller_link_back_interface const&) = delete ;
        controller_link_back_interface& operator =                     (controller_link_back_interface const&) = delete ;
    };

}}
