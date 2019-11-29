#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

#include <msw/noncopyable.hpp>
#include <msw/std/memory.hpp>
#include <msw/callback/event.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/asio/tcp_connectivity.hpp>
#include <msw/lexical_cast/bool2str.hpp>

#include <rubetek/log.hpp>
#include <rubetek/net/thrift_client.hpp>
#include <rubetek/proto/thrift/general.hpp>
#include <rubetek/client/remote/channels.hpp>
#include <rubetek/essence/unique_id/channel.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/server_connection_status.hpp>
#include <rubetek/client/remote/server_transceiver.hpp>
#include <rubetek/client/remote/unit_back_interface.hpp>

#include "cl_version_constants.h"


namespace rubetek {
namespace client  {

    struct remote_unit
    {
        typedef boost::asio::ip::tcp::resolver::query query               ;
        typedef proto::PacketType::type               server_command_type ;

             remote_unit           (
                                       boost::asio::io_service&
                                       , boost::optional<query>
                                       , bool use_ssl
                                       , unique_client_id::const_reference
                                       , remote_unit_back_interface*
                                   )                                               ;

        bool connected             ()                                        const ;

        void open_channel          (unique_controller_id::const_reference)         ;
        void close_channel         (unique_controller_id::const_reference)         ;

        bool has_channel           (unique_controller_id::const_reference)         ;

        void send_server_directly  (server_command_type, std::string const&)       ;
        void send_server_auxiliary (std::string const&)                            ;

    private:

        void set_connection_status (server_connection_status)                      ;
        void connection_broken     ()                                              ;

        typedef thrift_client<proto::Packet> thrift_client_type;

        logger                                 const logger_            ;
        remote_unit_back_interface*            const back_interface_    ;
        server_connection_status                     connection_status_ ;
        std::unique_ptr<msw::tcp_connectivity>       tcp_connectivity_  ;
        remote_channels                              remote_channels_   ;
        std::unique_ptr<thrift_client_type>          thrift_client_     ;
        thrift_general                               thrift_general_    ;
        server_transceiver                           transceiver_       ;
    };

}}


namespace rubetek {
namespace client  {

    inline remote_unit::remote_unit
    (
        boost::asio::io_service&            io_service
        , boost::optional<query>            query
        , bool                              use_ssl
        , unique_client_id::const_reference client_id
        , remote_unit_back_interface*       back_interface
    )
        : logger_            ( "remote unit", log::level::info      )
        , back_interface_    ( back_interface                       )
        , connection_status_ ( server_connection_status::connecting )
        , remote_channels_
        (
            [this](unique_channel_id_type channel_id, std::string const& ip, unique_client_id::const_reference cc_id, remote_link_interface<>* remote_link)
            {
                logger_.debug("channel opened for controller: ", cc_id);
                back_interface_->remote_unit_on_channel_opened(cc_id, remote_link, ip, channel_id);
            }
            , [this](unique_channel_id_type id)
            {
                back_interface_->remote_unit_on_channel_closed(id);
            }
            , [this](unique_channel_id_type channel_id, msw::range<msw::byte const> payload)
            {
                transceiver_.send_payload(channel_id, payload);
            }
        )
        , thrift_client_
        (
            query
            ?
            msw::make_unique<thrift_client_type>
            (
                io_service
                , *query
                , [this](proto::Packet const& pkt)
                {
                    if (connection_status_ != server_connection_status::connected)
                    {
                        logger_.debug("successfully connected to server");
                        tcp_connectivity_.reset();
                        transceiver_.request_version();
                        set_connection_status(server_connection_status::connected);
                    }
                    thrift_general_.process_in_packet(pkt);
                }
                , [this, client_id]()
                {
                    logger_.debug("send client id: ", client_id);
                    proto::CL_Connect cl_connect;
                    cl_connect.__set_client_id(client_id);
                    thrift_general_.send(proto::PacketType::CONNECT, cl_connect);
                }
                , [this]()
                {
                    connection_broken();
                }
                , [this, &io_service]()
                {
                    if (!tcp_connectivity_)
                    {
                        tcp_connectivity_.reset
                        (
                            new msw::tcp_connectivity
                            (
                                io_service
                                , msw::tcp_connectivity::query("google.com", "80")
                                , [this](bool success)
                                {
                                    tcp_connectivity_.reset();
                                    logger_.info(success ? "server not avalible" : "internet not avalible");
                                    set_connection_status(success ? server_connection_status::connecting : server_connection_status::no_internet);
                                }
                            )
                        );
                    }
                }
                , use_ssl
            )
            :
            nullptr
        )
        , thrift_general_
        (
            io_service
            , proto::Sender::CLIENT
            , [this](proto::Packet const& pkt)
            {
                if (pkt.sender == proto::Sender::SERVER) transceiver_.process_in_command(pkt.type, pkt.payload);
                else logger_.error("unknown sender: ", pkt.sender);
            }
            , [this](proto::Packet const& pkt)
            {
                if (connected()) thrift_client_->send(pkt);
            }
            , [this](int current_version)
            {
                if (current_version == proto::g_cl_version_constants.cl_version) logger_.info("current version: yes");
                else
                {
                    logger_.warning("current version: no");
                    back_interface_->remote_unit_on_close_due_to_malfunction_proto();
                }
            }
            , [this]()
            {
                connection_broken();
                if (connected()) thrift_client_->reset();
            }
            , msw::chrono::seconds(5)
            , msw::chrono::seconds(15)
        )
        , transceiver_
        (
            thrift_general_
            , remote_channels_
            , [this](server_packet_type packet_type, std::string const& packet)
            {
                back_interface_->remote_unit_on_directly_packet(packet_type, packet);
            }
            , [this](std::string const& packet)
            {
                back_interface_->remote_unit_on_auxiliary_packet(packet);
            }
        )
    {
        logger_.info("use ssl: ", msw::yes_no(use_ssl));
    }

    inline bool remote_unit::connected() const
    {
        return thrift_client_ && thrift_client_->connected();
    }

    inline void remote_unit::open_channel(unique_controller_id::const_reference id)
    {
        logger_.debug("open channel to controller: ", id);
        transceiver_.send_open(id);
    }
    inline void remote_unit::close_channel(unique_controller_id::const_reference controller_id)
    {
        logger_.debug("close channel for controller: ", controller_id);
        remote_channels_.close(controller_id, [this, controller_id](unique_channel_id_type channel_id)
        {
            transceiver_.send_close(channel_id);
        });
    }

    inline bool remote_unit::has_channel(unique_controller_id::const_reference id)
    {
        bool const result = remote_channels_.has_channel(id);
        logger_.debug("has channel for controller: ", id, ": ", msw::yes_no(result));
        return result;
    }

    inline void remote_unit::send_server_directly(server_command_type type, std::string const& cmd)
    {
        transceiver_.send_directly(type, cmd);
    }
    inline void remote_unit::send_server_auxiliary(std::string const& cmd)
    {
        transceiver_.send_auxiliary(cmd);
    }

    inline void remote_unit::set_connection_status(server_connection_status status)
    {
        if (connection_status_ != status)
        {
            connection_status_ = status;
            back_interface_->remote_unit_on_connection_status(connection_status_);
        }
    }
    inline void remote_unit::connection_broken()
    {
        logger_.debug("connection is broken");
        set_connection_status(server_connection_status::connecting);
        remote_channels_.clear();
    }

}}
