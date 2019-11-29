#pragma once

#include <unordered_map>

#include <msw/asio/timer.hpp>

#include <rubetek/log.hpp>
#include <rubetek/config.hpp>
#include <rubetek/client/device_info.hpp>
#include <rubetek/local_link/selector.hpp>
#include <rubetek/client/controller_info.hpp>
#include <rubetek/client/controller/controller.hpp>
#include <rubetek/client/controller_availability.hpp>
#include <rubetek/client/controller/unit_back_interface.hpp>
#include <rubetek/client/controller/controller_back_interface.hpp>
#include <rubetek/client/controller/active_controller_back_interface.hpp>


namespace rubetek {
namespace client  {

    struct controllers_unit
        : controller_back_interface
    {
        explicit                              controllers_unit                       (
                                                                                           boost::asio::io_service& io_service
                                                                                         , controllers_unit_back_interface*
                                                                                     )                                             ;

        void                                  add_local_link                         (
                                                                                           unique_controller_id::const_reference
                                                                                         , local_link_selector*
                                                                                         , bool known
                                                                                     )                                             ;
        void                                  remove_local_link                      (unique_controller_id::const_reference)       ;

        void                                  add_remote_link                        (
                                                                                           unique_controller_id::const_reference
                                                                                         , remote_link_interface<>*
                                                                                         , std::string const& ip_address
                                                                                         , unique_channel_id_type const&
                                                                                     )                                             ;
        void                                  remove_remote_link                     (unique_controller_id::const_reference)       ;

        bool                                  has_controller                         (unique_controller_id::const_reference) const ;

        known_controller_list                 known_list                             ()                                      const ;
        unknown_controller_list               unknown_list                           ()                                      const ;

        void                                  add_known_controller                   (unique_controller_id::const_reference)       ;
        void                                  add_unknown_controller                 (unique_controller_id::const_reference)       ;
        void                                  remove_known_controller                (unique_controller_id::const_reference)       ;

        bool                                  can_connect_to                         (unique_controller_id::const_reference) const ;
        void                                  connect_to                             (
                                                                                           unique_controller_id::const_reference
                                                                                         , active_controller_back_interface*
                                                                                         , device_info const&
                                                                                         , unsigned           project_crc
                                                                                         , std::string const& project_alias
                                                                                     )                                             ;

        controller_info                       make_info                              (unique_controller_id::const_reference) const ;
        controller_availability               availability_of                        (unique_controller_id::const_reference) const ;
        std::string                           ip_address_of                          (unique_controller_id::const_reference) const ;

        bool                                  has_active_controller                  ()                                      const ;
        controller&                           active_controller                      ()                                            ;
        unique_controller_id::const_reference active_controller_id                   ()                                      const ;
        void                                  disconnect_active_controller           ()                                            ;

        void                                  set_auto_known                         (unique_controller_id::const_reference)       ;

    private:

        typedef std::unordered_map<unique_controller_id::type, std::unique_ptr<controller>> controllers;

        void                                  idle                                   ()                                            ;

        controllers::iterator                 get_iterator                           (unique_controller_id::const_reference)       ;
        controllers::const_iterator           get_iterator                           (unique_controller_id::const_reference) const ;
        controllers::iterator                 add_controller                         (
                                                                                           unique_controller_id::const_reference
                                                                                         , bool known = false
                                                                                     )                                             ;
        void                                  on_remove_from_controller              (unique_controller_id::const_reference)       ;
        void                                  try_remove                             (controllers::iterator)                       ;

        void                                  controller_on_change_status            (unique_controller_id::const_reference) final ;
        void                                  controller_on_switch_links             (unique_controller_id::const_reference) final ;
        void                                  controller_on_change_remote_connection (unique_controller_id::const_reference) final ;
        void                                  controller_on_make_known               (unique_controller_id::const_reference) final ;
        void                                  controller_on_removed_from_controller  (unique_controller_id::const_reference) final ;
        void                                  controller_on_set_offline              (unique_controller_id::const_reference) final ;
        void                                  controller_on_rename                   (unique_controller_id::const_reference) final ;

        logger                           const logger_                          ;
        controllers_unit_back_interface* const back_interface_                  ;
        controllers                            controllers_                     ;
        controllers::iterator                  active_controller_               ;
        std::list<controllers::iterator>       potentially_removal_controllers_ ;
        msw::timer                             timer_                           ;
    };

}}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/controller/impl/unit.cpp>
#endif
