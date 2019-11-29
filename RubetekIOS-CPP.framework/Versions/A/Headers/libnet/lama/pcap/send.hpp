#pragma once

#include <msw/range.hpp>
#include <msw/throw_runtime_error.hpp>

#include <lama/pcap/handle.hpp>
#include <lama/pcap/include/pcap.hpp>


namespace lama {
namespace pcap {

    inline void send(handle const& h, msw::range<msw::byte const> packet)
    {
        if (::pcap_sendpacket(h.native(), packet.data().native(), static_cast<int>(packet.size().count())))
            msw::throw_runtime_error("error send packet: ", ::pcap_geterr(h.native()));
    }

}}
