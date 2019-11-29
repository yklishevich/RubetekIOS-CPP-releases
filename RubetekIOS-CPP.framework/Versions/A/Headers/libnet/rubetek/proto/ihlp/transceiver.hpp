#pragma once

#include <functional>

#include <msw/config.hpp>
#include <msw/range.hpp>
#include <msw/std/chrono.hpp>
#include <msw/noncopyable.hpp>
#include <msw/callback/event.hpp>
#include <msw/callback/byte_range.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/proto/spp/packer.hpp>
#include <msw/proto/spp/unpacker.hpp>
#include <msw/throw_runtime_error.hpp>

#include <rubetek/log.hpp>
#include <rubetek/proto/ihlp/timeouts.hpp>


#ifdef RUBETEK_IHLP_TRANSCEIVER_BASE_LOG
#   define RUBETEK_IHLP_TRANSCEIVER_BASE_ON_LOG(...) logger_.debug(__VA_ARGS__);
#else
#   define RUBETEK_IHLP_TRANSCEIVER_BASE_ON_LOG(...)
#endif


namespace rubetek {
namespace ihlp    {

    template <typename SizeType>
    struct transceiver_base
        : msw::noncopyable
    {
        typedef msw::callback::byte_range payload_ready;

        transceiver_base
        (
              payload_ready pay_ready
            , payload_ready tcp_payload_ready
            , timeouts      timeouts
        )
            : logger_          (
                                      "ihlp"
                                    , this
#ifdef RUBETEK_IHLP_TRANSCEIVER_BASE_LOG
                                    , log::level::debug
#endif
                               )
            , logger_io_       ( logger_                          )
            , creation_time_   ( msw::chrono::steady_clock::now() )
            , payload_ready_   ( pay_ready                        )
            , response_time_   ( timeouts.response                )
            , keep_alive_time_ ( timeouts.keep_alive              )
            , wait_response_   ( false                            )
            , spp_packer_      ( tcp_payload_ready                )
            , spp_unpacker_
            (
                [this](msw::range<msw::byte const> pkt)
                {
                    msw::buffer<msw::byte> tmp(pkt); /*fix error on arm*/
                    zzz_process_packet(tmp.all());
                }
            )
        {
            logger_.debug( "response time: "   , std::chrono::duration_cast<std::chrono::seconds>(response_time_  ).count(), " s" );
            logger_.debug( "keep-alive time: " , std::chrono::duration_cast<std::chrono::seconds>(keep_alive_time_).count(), " s" );
            zzz_wait_sending_keep_alive();
        }
        ~transceiver_base()
        {}

        void process_tcp_payload(msw::range<msw::byte const> packet)
        {
            spp_unpacker_.unpack(packet);
        }

        void send(msw::range<msw::byte const> packet)
        {
            logger_io_.send(packet);
            spp_packer_.add_packet(packet_type::payload, packet);
            zzz_wait_reception_response();
        }

        bool check_timeout()
        {
            RUBETEK_IHLP_TRANSCEIVER_BASE_ON_LOG("check_timeout");
            msw::chrono::steady_clock::time_point const now = msw::chrono::steady_clock::now();
            if (next_reaction_time_ < now)
            {
                if (wait_response_)
                {
                    RUBETEK_IHLP_TRANSCEIVER_BASE_ON_LOG("return false");
                    return false;
                }
                else
                {
                    RUBETEK_IHLP_TRANSCEIVER_BASE_ON_LOG("send keep_alive");
                    zzz_send_service_packet(packet_type::keep_alive);
                    zzz_wait_reception_response();
                }
            }
            else
            {
                RUBETEK_IHLP_TRANSCEIVER_BASE_ON_LOG("remaining time: ", std::chrono::duration_cast<std::chrono::seconds>(next_reaction_time_ - now).count(), " s");
            }
            return true;
        }

        bool is_early(msw::chrono::steady_clock::duration duration) const
        {
            return (msw::chrono::steady_clock::now() - creation_time_) > duration;
        }

        void set_keep_alive_time(msw::chrono::steady_clock::duration keep_alive_time)
        {
            keep_alive_time_ = keep_alive_time;
            if (!wait_response_) zzz_wait_sending_keep_alive();
        }

    private:

        enum class packet_type : msw::byte
        {
              keep_alive
            , payload
            , response
        };

        void zzz_process_packet(msw::range<msw::byte> packet)
        {
            if (packet.has_items())
            {
                zzz_wait_sending_keep_alive();
                packet_type pkt_type = packet.pop_front<packet_type>();
                switch (pkt_type)
                {
                    case packet_type::keep_alive:
                        zzz_send_service_packet(packet_type::response);
                        break;
                    case packet_type::payload:
                        try
                        {
                            logger_io_.received(packet);
                            payload_ready_(packet);
                        }
                        catch (std::exception const& e)
                        {
                            msw::throw_runtime_error("IHLP: payload process fail: ", e.what());
                        }
                        if (!wait_response_) zzz_send_service_packet(packet_type::response);
                        break;
                    case packet_type::response:
                        break;
                    default:
                        msw::throw_runtime_error("IHLP: bad packet type: ", static_cast<int>(pkt_type));
                }
            }
            else msw::throw_runtime_error("IHLP: bad packet size: ", packet.size());
        }

        void zzz_wait_sending_keep_alive()
        {
            wait_response_.reset();
            zzz_set_next_reaction_time(keep_alive_time_);
        }
        void zzz_wait_reception_response()
        {
            wait_response_.set();
            zzz_set_next_reaction_time(response_time_);
        }

        void zzz_set_next_reaction_time(msw::chrono::steady_clock::duration duration)
        {
            next_reaction_time_ = msw::chrono::steady_clock::now() + duration;
        }
        void zzz_send_service_packet(packet_type pkt_type)
        {
            spp_packer_.add_packet(msw::make_range<msw::byte>(pkt_type));
        }

        logger                                const logger_             ;
        logger_io<log_io_tag_ihlp>            const logger_io_          ;
        msw::chrono::steady_clock::time_point const creation_time_      ;
        payload_ready                               payload_ready_      ;
        msw::chrono::steady_clock::duration   const response_time_      ;
        msw::chrono::steady_clock::duration         keep_alive_time_    ;
        msw::chrono::steady_clock::time_point       next_reaction_time_ ;
        msw::trigger                                wait_response_      ;
        msw::spp::packer<SizeType>                  spp_packer_         ;
        msw::spp::unpacker<SizeType>                spp_unpacker_       ;
    };

    typedef transceiver_base<msw::wbyte> transceiver       ;
    typedef transceiver_base<msw::qbyte> large_transceiver ;

}}
