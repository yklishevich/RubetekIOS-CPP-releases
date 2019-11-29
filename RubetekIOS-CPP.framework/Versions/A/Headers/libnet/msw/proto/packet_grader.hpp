#pragma once

#include <functional>

#include <msw/config.hpp>

#ifdef MSW_MSVC
#   include <unordered_map>
#else
#   include <map>
#endif

#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/int_to/hex_std_string.hpp>


namespace msw
{
    template <typename PacketType = byte, typename ByteType = byte>
    struct packet_grader
        : noncopyable
    {
        typedef PacketType                                   packet_type          ;
        typedef ByteType                                     byte_type            ;
        typedef range<byte_type>                             block_type           ;
        typedef std::function<void(block_type)>              payload_ready        ;
        typedef std::function<void(packet_type, block_type)> on_unexpected_packet ;
        typedef std::
#ifdef MSW_MSVC
            unordered_map
#else
            map
#endif
                <packet_type, payload_ready> packets_map;

        static_assert(sizeof(packet_type) <= 4, "illogical large size of packet type");

        explicit packet_grader (packets_map&&      , on_unexpected_packet = nullptr) ;
        explicit packet_grader (packets_map const& , on_unexpected_packet = nullptr) ;

        void     process       (block_type)                                          ;

    private:
        static size<byte> const packet_type_size;

        packets_map          packets_map_          ;
        on_unexpected_packet on_unexpected_packet_ ;
    };

    template <typename PacketType, typename ByteType>
    size<byte> const packet_grader<PacketType, ByteType>::packet_type_size = size<packet_type>(1);
}


namespace msw
{
    template <typename PacketType, typename ByteType>
    packet_grader<PacketType, ByteType>::packet_grader(packets_map&& packets_map, on_unexpected_packet on_unexpected_packet)
        : packets_map_          ( std::move(packets_map) )
        , on_unexpected_packet_ ( on_unexpected_packet   )
    {}
    template <typename PacketType, typename ByteType>
    packet_grader<PacketType, ByteType>::packet_grader(packets_map const& packets_map, on_unexpected_packet on_unexpected_packet)
        : packets_map_          ( packets_map          )
        , on_unexpected_packet_ ( on_unexpected_packet )
    {}
    template <typename PacketType, typename ByteType>
    void packet_grader<PacketType, ByteType>::process(block_type packet)
    {
        if (packet.size() < packet_type_size) throw_runtime_error("very short packet len: ", packet.size(), " (need at least ", packet_type_size, ")");
        packet_type const pkt_t = packet.template pop_front<packet_type>();
        auto it = packets_map_.find(pkt_t);
        if (it == packets_map_.end())
        {
            if (on_unexpected_packet_) on_unexpected_packet_(pkt_t, packet);
            else throw_runtime_error("unexpected packet type: 0x", int_to_hex_std_string(static_cast<int>(pkt_t)));
        }
        it->second(packet);
    }
}
