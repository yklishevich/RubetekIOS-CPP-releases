#pragma once

#include <functional>

#include <msw/config.hpp>
#include <msw/range.hpp>
#include <msw/std/memory.hpp>
#include <msw/noncopyable.hpp>
#include <msw/callback/event.hpp>
#include <msw/callback/byte_range.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/proto/elastic_receiver.hpp>
#include <msw/regibits/transform/byte_reverse.hpp>

#include <rubetek/config.hpp>
#include <rubetek/log.hpp>
#include <rubetek/proto/thrift/serialize.hpp>


#ifdef RUBETEK_THRIFT_TRANSCEIVER_LOG
#   define RUBETEK_THRIFT_TRANSCEIVER_ON_LOG(...) logger_.debug(__VA_ARGS__);
#else
#   define RUBETEK_THRIFT_TRANSCEIVER_ON_LOG(...)
#endif


namespace rubetek
{
    template <typename Pkt>
    struct thrift_transceiver
        : msw::noncopyable
    {
        typedef std::function<void(Pkt const&)> packet_ready      ;
        typedef msw::callback::byte_const_range tcp_payload_ready ;
        typedef msw::callback::event            on_thrift_error   ;

        thrift_transceiver
        (
              packet_ready      packet_ready
            , tcp_payload_ready tcp_payload_ready
            , on_thrift_error   on_thrift_error
        )
            : logger_            (
                                      "thrift transceiver"
                                      , this
#ifdef RUBETEK_THRIFT_TRANSCEIVER_LOG
                                      , log::level::debug
#endif
                                 )
            , packet_ready_      ( packet_ready      )
            , tcp_payload_ready_ ( tcp_payload_ready )
            , on_thrift_error_   ( on_thrift_error   )
            , thrift_error_      ( false             )
        {
            reset();
        }

        void process_tcp_payload(msw::range<msw::byte const> packet)
        {
            RUBETEK_THRIFT_TRANSCEIVER_ON_LOG("process tcp payload");
            elastic_receiver_->external_on_receive(packet);
            if (thrift_error_)
            {
                reset            ();
                on_thrift_error_ ();
            }
            RUBETEK_THRIFT_TRANSCEIVER_ON_LOG("process tcp payload exit");
        }

        void send(Pkt const& pkt)
        {
            tcp_payload_ready_(msw::make_range<msw::byte>(thrift_serialize(pkt)));
        }

        void reset()
        {
            logger_.info("reset");
            thrift_error_     = false;
            elastic_receiver_ = msw::make_unique<msw::elastic_receiver<msw::byte const>>();
            receive_packet();
        }

    private:

        void receive_packet()
        {
            RUBETEK_THRIFT_TRANSCEIVER_ON_LOG("receive packet");
            elastic_receiver_->receive
            (
                msw::size<msw::byte>(4)
                , [this](msw::range<msw::byte const> hdr_len)
                {
                    msw::uint32 len;
                    msw::copy(hdr_len, len);
                    msw::size<msw::byte> const packet_len = msw::regibits::byte_reverse(len);
                    RUBETEK_THRIFT_TRANSCEIVER_ON_LOG("packet len: ", packet_len, " B");
                    if (packet_len.null()) receive_packet();
                    else
                    {
                        elastic_receiver_->receive_still
                        (
                            packet_len
                            , [this](msw::range<msw::byte const> packet)
                            {
                                RUBETEK_THRIFT_TRANSCEIVER_ON_LOG("packet ready: ", packet.size(), "B");
                                if (!thrift_error_)
                                {
                                    thrift_error_ = !deserialize_packet(packet);
                                    if (!thrift_error_) receive_packet();
                                }
                                else logger_.error("thrift error");
                            }
                        );
                    }
                }
            );
            RUBETEK_THRIFT_TRANSCEIVER_ON_LOG("receive packet exit");
        }

        bool deserialize_packet(msw::range<msw::byte const> packet)
        {
            RUBETEK_THRIFT_TRANSCEIVER_ON_LOG("deserialize packet");
            Pkt pkt;
            boost::optional<std::string> error;
            thrift_deserialize<Pkt>(msw::bytes_as_string(packet), pkt, error);
            if (error)
            {
                logger_.error("thrift deserialize: ", *error);
                return false;
            }
            packet_ready_(pkt);
            return true;
        }

        logger                                                  const logger_            ;
        packet_ready                                                  packet_ready_      ;
        tcp_payload_ready                                             tcp_payload_ready_ ;
        on_thrift_error                                               on_thrift_error_   ;
        bool                                                          thrift_error_      ;
        std::unique_ptr<msw::elastic_receiver<msw::byte const>>       elastic_receiver_  ;
    };
}
