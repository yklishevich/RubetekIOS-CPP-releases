#pragma once

#include <functional>

#include <msw/config.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>

#include <lama/thrift/include/thrift.hpp>
#include <lama/thrift/include/protocol/tprotocol.hpp>

#include <rubetek/log.hpp>
#include <rubetek/proto/thrift/general.hpp>
#include <rubetek/essence/unique_id/channel.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/remote/channels.hpp>
#include <rubetek/client/remote/server_packet_type.hpp>

#include "general_types.h"
#include "general_constants.h"
#include "cl_protocol_types.h"
#include "cl_protocol_constants.h"
#include "cl_types.h"
#include "cl_constants.h"


#ifdef RUBETEK_CLIENT_SERVER_TRANSCEIVER_LOG
#   define RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG(...) logger_.debug(__VA_ARGS__);
#else
#   define RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG(...)
#endif


namespace rubetek {
namespace client  {

    struct server_transceiver
        : msw::noncopyable
    {
        typedef std::function<void(server_packet_type, std::string const&)> directly_packet_ready  ;
        typedef std::function<void(std::string const&)>                     auxiliary_packet_ready ;

             server_transceiver      (thrift_general&, remote_channels&, directly_packet_ready, auxiliary_packet_ready) ;

        void process_in_command      (proto::PacketType::type, std::string const& payload)                              ;

        void request_version         ()                                                                                 ;

        void send_open               (unique_controller_id::const_reference)                                            ;
        void send_close              (unique_channel_id_type)                                                           ;
        void send_payload            (unique_channel_id_type, msw::range<msw::byte const>)                              ;

        void send_directly           (server_packet_type, std::string const&)                                           ;
        void send_auxiliary          (std::string const&)                                                               ;

    private:

        void process_in_command_impl (proto::PacketType::type packet_type, std::string const& payload)                  ;

        logger                 const logger_                 ;
        directly_packet_ready  const directly_packet_ready_  ;
        auxiliary_packet_ready const auxiliary_packet_ready_ ;
        thrift_general&              thrift_general_         ;
        remote_channels&             remote_channels_        ;
    };

}}


namespace rubetek {
namespace client {

    inline server_transceiver::server_transceiver
    (
        thrift_general&          thrift_general
        , remote_channels&       remote_channels
        , directly_packet_ready  directly_packet_ready
        , auxiliary_packet_ready auxiliary_packet_ready
    )
        : logger_                 (
                                       "server transceiver"
                                       , this
#ifdef RUBETEK_CLIENT_SERVER_TRANSCEIVER_LOG
                                       , log::level::debug
#endif
                                  )
        , directly_packet_ready_  ( directly_packet_ready  )
        , auxiliary_packet_ready_ ( auxiliary_packet_ready )
        , thrift_general_         ( thrift_general         )
        , remote_channels_        ( remote_channels        )
    {}

    inline void server_transceiver::process_in_command(proto::PacketType::type packet_type, std::string const& payload)
    {
        try
        {
            process_in_command_impl(packet_type, payload);
        }
        catch (std::exception const& e)
        {
            logger_.error("input command: ", e.what());
            throw;
        }
    }

    inline void server_transceiver::request_version()
    {
        thrift_general_.send_version();
    }

    inline void server_transceiver::send_open(unique_controller_id::const_reference id)
    {
        RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG("send open: ", id);
        proto::CL_OpenSession cl_open_session;
        cl_open_session.__set_controller_id(id);
        thrift_general_.send(proto::PacketType::OPEN, cl_open_session);
    }
    inline void server_transceiver::send_close(unique_channel_id_type id)
    {
        RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG("send close: ", id);
        proto::CloseSession close_session;
        close_session.__set_session_id(id);
        thrift_general_.send(proto::PacketType::CLOSE, close_session);
    }
    inline void server_transceiver::send_payload(unique_channel_id_type id, msw::range<msw::byte const> packet)
    {
        RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG("send payload: ", id);
        proto::Payload pay;
        pay.__set_session_id(id);
        pay.__set_data(msw::bytes_as_string(packet));
        thrift_general_.send(proto::PacketType::PAYLOAD, pay);
    }

    inline void server_transceiver::send_directly(proto::PacketType::type packet_type, std::string const& pay)
    {
        RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG("send directly");
        thrift_general_.send(packet_type, pay);
    }
    inline void server_transceiver::send_auxiliary(std::string const& pay)
    {
        RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG("send auxiliary");
        thrift_general_.send(proto::PacketType::AUXILIARY, pay);
    }

    inline void server_transceiver::process_in_command_impl(proto::PacketType::type packet_type, std::string const& payload)
    {
        switch (packet_type)
        {
            case proto::PacketType::SESSION:
            {
                RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG("recv open session");
                proto::CL_Session cl_session;
                thrift_deserialize(payload, cl_session);
                remote_channels_.on_open(cl_session.session_id, cl_session.controller_id, cl_session.controller_ip);
            }
            break;
            case proto::PacketType::CLOSE:
            {
                RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG("recv close session");
                proto::CloseSession close_session;
                thrift_deserialize(payload, close_session);
                remote_channels_.on_close(close_session.session_id);
            }
            break;
            case proto::PacketType::PAYLOAD:
            {
                RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG("recv payload");
                proto::Payload pay;
                thrift_deserialize(payload, pay);
                remote_channels_.on_payload(pay.session_id, msw::make_range<msw::byte>(pay.data));
            }
            break;
            case proto::PacketType::AUXILIARY:
            {
                RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG("recv auxiliary");
                auxiliary_packet_ready_(payload);
            }
            break;
            default:
            {
                RUBETEK_CLIENT_SERVER_TRANSCEIVER_ON_LOG("recv directly");
                directly_packet_ready_(packet_type, payload);
            }
        }
    }

}}
