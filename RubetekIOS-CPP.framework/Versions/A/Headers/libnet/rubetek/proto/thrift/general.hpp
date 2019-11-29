#pragma once

#include <functional>

#include <boost/asio.hpp>

#include <msw/range.hpp>
#include <msw/config.hpp>
#include <msw/asio/timer.hpp>
#include <msw/std/chrono.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/callback/byte_range.hpp>

#include <lama/thrift/include/thrift.hpp>
#include <lama/thrift/include/protocol/tprotocol.hpp>

#include <rubetek/config.hpp>
#include <rubetek/proto/thrift/serialize.hpp>

#include "version_types.h"
#include "version_constants.h"
#include "general_types.h"
#include "general_constants.h"


#ifdef RUBETEK_THRIFT_GENERAL_LOG
#   define RUBETEK_THRIFT_GENERAL_ON_LOG(...) logger_.debug(__VA_ARGS__);
#else
#   define RUBETEK_THRIFT_GENERAL_ON_LOG(...)
#endif


namespace rubetek
{
    struct thrift_general
        : msw::noncopyable
    {
        typedef std::function<void(proto::Packet const&)> packet_ready        ;
        typedef std::function<void(int)>                  on_check_version    ;
        typedef std::function<void()>                     on_response_timeout ;

        thrift_general
        (
              boost::asio::io_service&            io_service
            , proto::Sender::type                 sender
            , packet_ready                        in_packet_ready
            , packet_ready                        out_packet_ready
            , on_check_version                    on_check_version
            , on_response_timeout                 on_response_timeout
            , msw::chrono::steady_clock::duration ping_interval    = msw::chrono::seconds(15)
            , msw::chrono::steady_clock::duration response_timeout = msw::chrono::seconds(40)
        )
            : logger_              (
                                          "thrift general"
                                        , this
#ifdef RUBETEK_THRIFT_GENERAL_LOG
                                        , log::level::debug
#endif
                                   )
            , sender_              ( sender                           )
            , ping_interval_       ( ping_interval                    )
            , response_timeout_    ( response_timeout                 )
            , in_packet_ready_     ( in_packet_ready                  )
            , out_packet_ready_    ( out_packet_ready                 )
            , on_check_version_    ( on_check_version                 )
            , on_response_timeout_ ( on_response_timeout              )
            , last_response_time_  ( msw::chrono::steady_clock::now() )
            , timer_
            (
                io_service
                , msw::chrono::milliseconds(500)
                , [this]()
                {
                    idle();
                }
            )
        {
            logger_.info( "ping interval: "   , std::chrono::duration_cast<std::chrono::seconds>( ping_interval_    ).count(), " s" );
            logger_.info( "response timeout: ", std::chrono::duration_cast<std::chrono::seconds>( response_timeout_ ).count(), " s" );
            if (ping_interval_ >= response_timeout_) msw::throw_runtime_error("ping interval must be less then response timeout");
        }

        void process_in_packet(proto::Packet const& pkt)
        {
            RUBETEK_THRIFT_GENERAL_ON_LOG("process input packet");
            try
            {
                last_response_time_ = msw::chrono::steady_clock::now();
                switch (pkt.type)
                {
                    case proto::PacketType::VER:
                    {
                        RUBETEK_THRIFT_GENERAL_ON_LOG("recv version");
                        proto::Version version;
                        thrift_deserialize(pkt.payload, version);
                        on_check_version_(version.value);
                    }
                    break;
                    case proto::PacketType::PING:
                        RUBETEK_THRIFT_GENERAL_ON_LOG("recv ping");
                    break;
                    case proto::PacketType::RESPONSE:
                    {
                        RUBETEK_THRIFT_GENERAL_ON_LOG("recv response");
                        proto::Response response;
                        thrift_deserialize(pkt.payload, response);
                        if (response.code != proto::StatusCode::OK) logger_.error("response error: ", response.message);
                    }
                    return;
                    default: in_packet_ready_(pkt);
                };
            }
            catch (std::exception const& e)
            {
                logger_.error("incoming packet: ", e.what());
                proto::Response response;
                response.__set_code    ( proto::StatusCode::ERR );
                response.__set_message ( e.what()               );
                send(proto::PacketType::RESPONSE, response);
                return;
            }
            RUBETEK_THRIFT_GENERAL_ON_LOG("send response");
            proto::Response response;
            response.__set_code(proto::StatusCode::OK);
            send(proto::PacketType::RESPONSE, response);
        }

        template <typename Pay>
        void send(proto::PacketType::type type, Pay const& pay)
        {
            send(type, thrift_serialize(pay));
        }

        void send(proto::PacketType::type type, std::string const& pay)
        {
            RUBETEK_THRIFT_GENERAL_ON_LOG("send packet: ", static_cast<int>(type));
            proto::Packet pkt;
            pkt.__set_type    ( type    );
            pkt.__set_sender  ( sender_ );
            pkt.__set_payload ( pay     );
            out_packet_ready_(pkt);
        }

        void send_version()
        {
            RUBETEK_THRIFT_GENERAL_ON_LOG("send version");
            proto::Packet pkt;
            pkt.__set_type   ( proto::PacketType::VER );
            pkt.__set_sender ( sender_                );
            out_packet_ready_(pkt);
        }

        void ping()
        {
            RUBETEK_THRIFT_GENERAL_ON_LOG("send ping");
            proto::Packet pkt;
            pkt.__set_type   ( proto::PacketType::PING );
            pkt.__set_sender ( sender_                 );
            out_packet_ready_(pkt);
        }

        msw::chrono::steady_clock::duration ping_interval() const
        {
            return ping_interval_;
        }

    private:

        void idle()
        {
            if ((msw::chrono::steady_clock::now() - last_response_time_) > response_timeout_)
            {
                logger_.debug("waiting time for response has expired");
                on_response_timeout_();
                last_response_time_ = msw::chrono::steady_clock::now();
            }
            else if (next_ping_time_ < msw::chrono::steady_clock::now())
            {
                ping();
                next_ping_time_ = msw::chrono::steady_clock::now() + ping_interval_;
            }
        }

        logger                                const logger_              ;
        proto::Sender::type                   const sender_              ;
        msw::chrono::steady_clock::duration   const ping_interval_       ;
        msw::chrono::steady_clock::duration   const response_timeout_    ;
        packet_ready                          const in_packet_ready_     ;
        packet_ready                          const out_packet_ready_    ;
        on_check_version                      const on_check_version_    ;
        on_response_timeout                   const on_response_timeout_ ;
        msw::chrono::steady_clock::time_point       next_ping_time_      ;
        msw::chrono::steady_clock::time_point       last_response_time_  ;
        msw::timer                                  timer_               ;
    };
}
