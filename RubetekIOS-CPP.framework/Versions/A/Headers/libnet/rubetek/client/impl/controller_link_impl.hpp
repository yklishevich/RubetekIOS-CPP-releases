#pragma once

#include <string>
#include <memory>

#include <rubetek/config.hpp>

#include <msw/asio/timer.hpp>
#include <msw/preconstructor.hpp>
#include <msw/dimension/trigger.hpp>

#include <rubetek/log.hpp>

#include <rubetek/thread/io.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/essence/unique_id/client.hpp>
#include <rubetek/essence/unique_id/controller.hpp>

#include <rubetek/config.hpp>
#include <rubetek/client/local/unit2.hpp>
#include <rubetek/client/remote/unit.hpp>
#include <rubetek/client/controller/unit.hpp>
#include <rubetek/client/account_projects.hpp>
#include <rubetek/client/client/configurator.hpp>
#include <rubetek/client/client/project_manager.hpp>
#include <rubetek/client/client/callback_service.hpp>
#include <rubetek/client/controller_link_context.hpp>
#include <rubetek/client/local/unit_back_interface.hpp>
#include <rubetek/client/remote/server_packet_type.hpp>
#include <rubetek/client/controller_link_back_interface.hpp>
#include <rubetek/client/controller/unit_back_interface.hpp>
#include <rubetek/client/controller/active_controller_back_interface.hpp>


namespace rubetek {
namespace client  {

    struct controller_link_impl
        : controllers_unit_back_interface
        , local_unit_back_interface
        , remote_unit_back_interface
        , active_controller_back_interface
    {
             controller_link_impl     (
                                            controller_link_back_interface*
                                          , unique_client_id::const_reference
                                          , controller_link_context const&
                                          , bool                              focus
                                          , bool                              disable_start_logic
                                          , bool                              disable_find_on_empty
                                      )                                                               ;
        virtual // for gcc
             ~controller_link_impl    ()                                                              ;

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

        void send_project             (std::string)                                                   ;
        void send_password            (std::string)                                                   ;
        void send_property            (property_address, property)                                    ;
        void send_property            (property_address, property_list)                               ;
        void send_command             (msw::range<msw::byte const>)                                   ;
        void send_server_directly     (server_packet_type, std::string)                               ;
        void send_server_auxiliary    (std::string)                                                   ;
        void send_notification_id     (std::string)                                                   ;

        void remove_client            (unique_controller_id::const_reference)                         ;

        void synchronize_projects     (account_projects)                                              ;

        void process_events           ()                                                              ;

        static std::list<unique_controller_id::type>
             projects_id_list         (std::string const& work_path)                                  ;

    private:

        struct context
        {
            unique_client_id::type unique_id ;
            device_info            device    ;
        };

        enum class show_list
        {
              none
            , known
            , unknown
            , all
        };

        typedef project_manager<unique_controller_id::type> project_manager_type ;
        typedef boost::optional<unique_controller_id::type> wait_for_connect     ;

        bool        check_configuration                                     ()                                          const ;
        void        start                                                   (bool enable_find_on_empty)                       ;
        void        controller_activated                                    ()                                                ;
        void        active_controller_changed_connection_status             ()                                                ;
        void        remove_from_controller                                  (unique_controller_id::const_reference)           ;

        void        self_connect_to                                         (unique_controller_id::type)                      ;
        void        self_disconnect                                         ()                                                ;
        void        self_show_unknown_controllers                           ()                                                ;
        void        self_show_known_controllers                             ()                                                ;
        void        self_show_all_controllers                               ()                                                ;
        void        self_cancel_show_list                                   ()                                                ;
        void        self_synchronize_projects                               (account_projects const&)                         ;
        void        self_on_any_controller_change_state                     ()                                                ;

        void        controllers_unit_on_any_controller_change_status        ()                                          final ;
        void        controllers_unit_on_became_known                        (unique_controller_id::const_reference)     final ;
        void        controllers_unit_on_removed_from_controller             (unique_controller_id::const_reference)     final ;
        std::string controllers_unit_on_get_alias                           (unique_controller_id::const_reference)     final ;
        std::string controllers_unit_on_get_name                            (unique_controller_id::const_reference)     final ;
        void        controllers_unit_on_disconnected_active_controller      ()                                          final ;
        void        controllers_unit_on_active_controller_change_connection ()                                          final ;
        void        controllers_unit_on_loading_progress                    (int)                                       final ;
        void        controllers_unit_on_rename                              (unique_controller_id::const_reference)     final ;

        bool        local_unit_on_check_project                             (unique_controller_id::const_reference)     final ;
        void        local_unit_on_remove_project                            (unique_controller_id::const_reference)     final ;
        void        local_unit_on_add_controller                            (
                                                                                  unique_controller_id::const_reference
                                                                                , local_link_selector*
                                                                                , bool known
                                                                            )                                           final ;
        void        local_unit_on_remove_controller                         (unique_controller_id::const_reference)     final ;

        void        remote_unit_on_connection_status                        (server_connection_status)                  final ;
        void        remote_unit_on_channel_opened                           (
                                                                                  unique_controller_id::const_reference
                                                                                , remote_link_interface<>*
                                                                                , std::string const& ip
                                                                                , unique_channel_id_type const&
                                                                            )                                           final ;
        void        remote_unit_on_channel_closed                           (unique_channel_id_type)                    final ;
        void        remote_unit_on_directly_packet                          (server_packet_type, std::string const&)    final ;
        void        remote_unit_on_auxiliary_packet                         (std::string const&)                        final ;
        void        remote_unit_on_close_due_to_malfunction_proto           ()                                          final ;

        void        active_controller_on_query_password                     (option_was_removed)                        final ;
        void        active_controller_on_no_project                         ()                                          final ;
        void        active_controller_on_no_required_resolution             ()                                          final ;
        void        active_controller_on_update_project                     ()                                          final ;
        void        active_controller_on_loading_progress                   (int)                                       final ;
        void        active_controller_on_project                            (msw::range<msw::byte const>, unsigned crc) final ;
        void        active_controller_on_ready                              ()                                          final ;
        void        active_controller_on_property                           (property_address, property_list const&)    final ;
        void        active_controller_on_module_command                     (std::string const&)                        final ;

        logger                      const logger_                ;
        msw::preconstructor         const show_build_info_       ;
        context                     const context_               ;
        callback_service                  callback_service_      ;
        boost::asio::io_service           io_service_            ;
        configurator                      configurator_          ;
        project_manager_type              project_manager_       ;
        show_list                         show_list_             ;
        std::unique_ptr<int>              updating_progress_     ;
        wait_for_connect                  wait_for_connect_      ;
        msw::trigger                      find_first_controller_ ;
        controllers_unit                  controllers_unit_      ;
        local_unit                        local_unit_            ;
        remote_unit                       remote_unit_           ;
        std::unique_ptr<msw::timer>       start_timer_           ;
        msw::preconstructor               preconstructor_        ;
        msw::timer                        timer_                 ;
        io_thread                         io_thread_             ;
    };

}}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/impl/controller_link_impl.cpp>
#endif
