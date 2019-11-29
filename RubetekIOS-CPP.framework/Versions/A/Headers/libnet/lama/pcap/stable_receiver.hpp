#pragma once

#include <functional>

#include <msw/std/chrono.hpp>
#include <msw/std/thread.hpp>

#include <lama/pcap/next.hpp>
#include <lama/pcap/open.hpp>
#include <lama/pcap/stats.hpp>
#include <lama/pcap/handle.hpp>
#include <lama/pcap/default.hpp>


namespace lama {
namespace pcap {

    struct stable_receiver
    {
        typedef std::function<void(std::string const&)> on_error_t;

        explicit stable_receiver
        (
              std::string const& name
            , int                read_timeout = default_read_timeout
            , int                snaplen      = default_snaplen
            , int                flags        = default_open_flags
        )
            : name_         ( name         )
            , read_timeout_ ( read_timeout )
            , snaplen_      ( snaplen      )
            , flags_        ( flags        )
        {
            set_buffer_size(msw::MB * 64);
        }
        template <typename PacketReady>
        bool next(PacketReady packet_ready, on_error_t on_error = nullptr)
        {
            std::unique_ptr<std::string> error;
            if (!h_)
            {
                h_ = open(name_, error, read_timeout_, snaplen_, flags_);
                if (h_)
                {
                    error_.clear();
                    if (buffer_size_.not_null()) h_.set_buffer_size(buffer_size_);
                }
                else
                {
                    set_error(*error, on_error);
                    return false;
                }
            }
            packet_header header;
            msw::range<msw::byte const> packet = next_nothrow(h_, error, header);
            if (error)
            {
                h_ = handle();
                set_error(*error, on_error);
                return false;
            }
            else if (packet.has_items()) packet_ready(header, packet);
            return true;
        }
        template <typename PacketReady>
        void loop(PacketReady packet_ready, on_error_t on_error = nullptr)
        {
            for (;;) next(packet_ready, on_error);
        }
        void get_stats(stats& s)
        {
            if (h_) h_.get_stats(s);
            else s = {0};
        }
        void set_buffer_size(msw::size<msw::byte> s)
        {
            buffer_size_ = s;
            if (h_) h_.set_buffer_size(buffer_size_);
        }
        bool good() const
        {
            return static_cast<bool>(h_);
        }

    private:

        void set_error(std::string const& error, on_error_t on_error)
        {
            msw::this_thread::sleep_for(msw::chrono::milliseconds(read_timeout_));
            if (error_ != error)
            {
                error_ = error;
                if (on_error) on_error(error_);
            }
        }

        std::string          const name_         ;
        int                  const read_timeout_ ;
        int                  const snaplen_      ;
        int                  const flags_        ;
        msw::size<msw::byte>       buffer_size_  ;
        std::string                error_        ;
        handle                     h_            ;
    };

}}
