#pragma once

#include <functional>

#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw          {
namespace large_packet {

    struct wrapper
        : noncopyable
    {
        typedef std::function<void(range<byte>)> packet_ready;

             wrapper      (packet_ready, size<byte> max_packet_size) ;
        void add_packet   (range<byte const>)                        ;
    private:
        void flush_packet (bool not_last, range<byte const>)         ;

        packet_ready const packet_ready_ ;
        buffer<byte>       packet_       ;
    };

}}


namespace msw {
namespace large_packet {

    inline wrapper::wrapper(packet_ready packet_ready, size<byte> max_packet_size)
        : packet_ready_ ( packet_ready       )
        , packet_       ( 0, max_packet_size )
    {
        if (max_packet_size < 2) throw_runtime_error("large packet wrapper max packet size must be more then 2, but factual it equal ", max_packet_size.count());
    }
    inline void wrapper::add_packet(range<byte const> packet)
    {
        size<byte> const max_payload_size = packet_.capacity() - 1;
        for (;;)
        {
            if (packet.size() > max_payload_size)
            {
                add_packet(true, packet.pop_front(max_payload_size));
            }
            else
            {
                add_packet(false, packet);
                break;
            }
        }
    }

    inline void wrapper::add_packet(bool not_last, range<byte const> packet)
    {
        packet_.push_back_silence(byte(not_last));
        packet_.push_back_silence(packet);
        packet_ready_(packet_.all());
        packet_.clear();
    }

}}
