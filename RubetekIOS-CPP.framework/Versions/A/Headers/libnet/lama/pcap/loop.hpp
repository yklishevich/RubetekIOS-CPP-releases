#pragma once

#include <msw/range.hpp>

#include <lama/pcap/next.hpp>
#include <lama/pcap/handle.hpp>
#include <lama/pcap/include/pcap.hpp>
#include <lama/pcap/packet_header.hpp>


namespace lama {
namespace pcap {

    template <typename PacketReady>
    void loop(handle const& h, PacketReady&& packet_ready)
    {
        packet_header hdr;
        range<byte const> packet;
        for (;;)
        {
            packet = next(h, hdr);
            if (packet.has_items()) packet_ready(hdr, packet);
        }
    }

}}
