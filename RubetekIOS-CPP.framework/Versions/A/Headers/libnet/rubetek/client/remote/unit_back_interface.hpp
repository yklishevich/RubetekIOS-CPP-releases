#pragma once

#include <string>

#include <msw/noncopyable.hpp>

#include <rubetek/remote_link/interface.hpp>
#include <rubetek/essence/unique_id/channel.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/server_connection_status.hpp>
#include <rubetek/client/remote/server_packet_type.hpp>


namespace rubetek {
namespace client  {

    struct remote_unit_back_interface
        : msw::noncopyable
    {
        virtual void remote_unit_on_connection_status              (server_connection_status)                                                                                              = 0 ;
        virtual void remote_unit_on_channel_opened                 (unique_controller_id::const_reference, remote_link_interface<>*, std::string const& ip, unique_channel_id_type const&) = 0 ;
        virtual void remote_unit_on_channel_closed                 (unique_channel_id_type)                                                                                                = 0 ;
        virtual void remote_unit_on_directly_packet                (server_packet_type, std::string const&)                                                                                = 0 ;
        virtual void remote_unit_on_auxiliary_packet               (std::string const&)                                                                                                    = 0 ;
        virtual void remote_unit_on_close_due_to_malfunction_proto ()                                                                                                                      = 0 ;
    protected:
        ~remote_unit_back_interface() {}
    };

}}
