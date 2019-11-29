#pragma once

#include <array>
#include <queue>
#include <string>
#include <utility>
#include <functional>

#include <msw/std/mutex.hpp>

#include <rubetek/log.hpp>
#include <rubetek/config.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/client/was_removed.hpp>
#include <rubetek/client/project_error.hpp>
#include <rubetek/client/controller_info.hpp>
#include <rubetek/client/controller_error.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/server_connection_status.hpp>
#include <rubetek/client/remote/server_packet_type.hpp>
#include <rubetek/client/controller_link_back_interface.hpp>


namespace rubetek {
namespace client  {

    struct callback_service
        : controller_link_back_interface
    {
        explicit callback_service(controller_link_back_interface*);

        void on_find_controller              ()                                               final ;
        void on_unknown_controllers          (unknown_controller_list)                        final ;
        void on_known_controllers            (known_controller_list)                          final ;
        void on_all_controllers              (known_controller_list, unknown_controller_list) final ;
        void on_notify_about_location        (known_controller_list)                          final ;
        void on_server_connection_status     (server_connection_status)                       final ;
        void on_connecting_to_controller     (controller_info, std::string alias)             final ;
        void on_controller_connection_status (controller_availability, std::string ip)        final ;
        void on_controller_rename            (std::string const&)                             final ;
        void on_controller_error             (controller_error)                               final ;
        void on_password_request             (option_was_removed)                             final ;
        void on_project_error                (project_error)                                  final ;
        void on_project_updating             ()                                               final ;
        void on_ready                        (std::string project_file)                       final ;
        void on_properties_received          (property_address, property_list)                final ;
        void on_module_command               (std::string const&)                             final ;
        void on_server_directly              (server_packet_type, std::string const&)         final ;
        void on_server_auxiliary             (std::string const&)                             final ;
        void on_update_required              ()                                               final ;

        void run();

        void cancel_on_properties_received();

    private:

        typedef std::function<void()> event;

        struct property_block
        {
            property_address address ;
            property_list    list    ;
        };

#define RUBETEK_CLIENT_CALLBACK_SERVICE_FN(Name)\
    public: void cancel_##Name();\
    private: event& fn_##Name();
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_find_controller              )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_unknown_controllers          )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_known_controllers            )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_all_controllers              )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_notify_about_location        )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_server_connection_status     )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_connecting_to_controller     )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_controller_connection_status )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_controller_rename            )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_controller_error             )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_password_request             )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_project_error                )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_project_updating             )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_ready                        )
        RUBETEK_CLIENT_CALLBACK_SERVICE_FN(on_update_required              )
#undef RUBETEK_CLIENT_CALLBACK_SERVICE_FN

        //typedef msw::recursive_mutex mutex_type;
        typedef msw::mutex mutex_type;

        logger                                                 const logger_                   ;
        controller_link_back_interface*                        const back_interface_           ;
        mutex_type                                                   mutex_                    ;
        std::array<std::pair<std::string, event>, 15>                events_                   ;
        std::queue<property_block>                                   property_blocks_          ;
        std::queue<std::string>                                      module_commands_          ;
        std::queue<std::pair<server_packet_type, std::string>>       server_directly_packets_  ;
        std::queue<std::string>                                      server_auxiliary_packets_ ;
    };

}}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/client/impl/callback_service.cpp>
#endif
