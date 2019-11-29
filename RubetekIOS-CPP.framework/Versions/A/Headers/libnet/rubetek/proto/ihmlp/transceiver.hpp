#pragma once

#include <functional>

#include <msw/config.hpp>
#include <msw/buffer.hpp>
#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/dimension/counter.hpp>
#include <msw/callback/byte_range.hpp>
#include <msw/throw_runtime_error.hpp>

#include <rubetek/log.hpp>


namespace rubetek {
namespace ihmlp   {

    struct transceiver
        : msw::noncopyable
    {
        typedef msw::callback::byte_const_range payload_ready          ;
        typedef msw::callback::byte_const_range ihmlp_packet_ready     ;
        typedef msw::callback::event            all_links_disconnected ;

        transceiver(payload_ready payload_ready, all_links_disconnected all_links_disconnected)
            : logger_                 ( "ihmlp", this          )
            , logger_io_              ( logger_                )
            , init_                   ( true                   )
            , buf_                    ( 0, msw::KB * 64        )
            , payload_ready_          ( payload_ready          )
            , all_links_disconnected_ ( all_links_disconnected )
        {}

        void set_primary_link(ihmlp_packet_ready ihmlp_packet_ready)
        {
            logger_.debug("set primary link");
            primary_ihmlp_packet_ready_ = ihmlp_packet_ready;
        }
        void reset_primary_link()
        {
            logger_.debug("reset primary link");
            primary_ihmlp_packet_ready_ = nullptr;
            zzz_if_no_links();
            if (buf_.has_items() && secondary_ihmlp_packet_ready_) secondary_ihmlp_packet_ready_(buf_.all());
        }
        void set_secondary_link(ihmlp_packet_ready ihmlp_packet_ready)
        {
            logger_.debug("set secondary link");
            secondary_ihmlp_packet_ready_ = ihmlp_packet_ready;
        }
        void reset_secondary_link()
        {
            logger_.debug("reset secondary link");
            secondary_ihmlp_packet_ready_ = nullptr;
            zzz_if_no_links();
        }

        bool has_links() const
        {
            return primary_ihmlp_packet_ready_ || secondary_ihmlp_packet_ready_;
        }
        bool has_primary_link() const
        {
            return static_cast<bool>(primary_ihmlp_packet_ready_);
        }

        void process_ihmlp_packet(msw::range<msw::byte const> packet)
        {
            //logger_.debug("process_ihmlp_packet");
            MSW_ASSERT(packet.has_items());
            if (packet.has_items())
            {
                header const hdr = packet.pop_front<header>();
                if (hdr == header::init)
                {
                    init_.reset();
                    zzz_reset();
                }
                if (packet.has_items())
                {
                    msw::byte const cntr_val = packet.pop_front();
                    if (cntr_val == in_counter_.value())
                    {
                        logger_.warning("received packet with the repetitive counter value: ", int(cntr_val));
                    }
                    else
                    {
                        if (cntr_val != msw::byte(in_counter_.value() + 1))
                            logger_.error("bad counter value: ", int(cntr_val), " (inside: ", int(in_counter_.value()),")");
                        in_counter_.set(cntr_val);
                        buf_.clear();
                        try
                        {
                            logger_io_.received(packet);
                            payload_ready_(packet);
                        }
                        catch (std::exception const& e)
                        {
                            msw::throw_runtime_error("IHMLP: payload process fail: ", e.what());
                        }
                        if (packet.has_items() && buf_.empty()) zzz_send_ihmlp_packet();
                    }
                }
            }
            //logger_.debug("process_ihmlp_packet exit");
        }

        bool can_send() const
        {
            return buf_.empty();
        }

        void send_packet(msw::range<msw::byte const> packet)
        {
            //logger_.debug("send_packet");
            if (!has_links()) throw std::runtime_error("IHMLP: no available links");
            if (packet.empty()) throw std::runtime_error("IHMLP: packet is empty");
            if (buf_.has_items()) throw std::runtime_error("IHMLP: have not yet received confirmation of acceptance of the previous packet");
            zzz_send_ihmlp_packet(packet);
            //logger_.debug("send_packet exit");
        }

    private:

        enum class header : msw::byte
        {
            init = 0x66
            , payload = 0xff
        };

        void zzz_send_ihmlp_packet(msw::range<msw::byte const> packet = msw::range<msw::byte const>())
        {
            MSW_ASSERT(has_links());
            MSW_ASSERT(buf_.empty());
            logger_io_.send(packet);
            buf_.push_back(msw::byte(init_ ? header::init : header::payload));
            init_.reset();
            buf_.push_back((++out_counter_).value());
            if (packet.has_items()) buf_.push_back(packet);
            if (primary_ihmlp_packet_ready_)
                primary_ihmlp_packet_ready_(buf_.all());
            else
                secondary_ihmlp_packet_ready_(buf_.all());
            if (packet.empty()) buf_.clear();
        }

        void zzz_reset()
        {
            logger_.debug("reset");
            in_counter_.reset();
            out_counter_.reset();
            buf_.clear();
        }

        void zzz_if_no_links()
        {
            if (!has_links())
            {
                init_.set();
                zzz_reset();
                all_links_disconnected_();
            }
        }

        logger                      const logger_                       ;
        logger_io<log_io_tag_ihmlp> const logger_io_                    ;
        msw::trigger                      init_                         ;
        msw::counter<msw::byte>           in_counter_                   ;
        msw::counter<msw::byte>           out_counter_                  ;
        msw::buffer<msw::byte>            buf_                          ;
        payload_ready                     payload_ready_                ;
        msw::callback::event              all_links_disconnected_       ;
        ihmlp_packet_ready                primary_ihmlp_packet_ready_   ;
        ihmlp_packet_ready                secondary_ihmlp_packet_ready_ ;
    };

}}
