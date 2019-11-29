#pragma once

#include <string>

#include <msw/config.hpp>
#include <msw/plain_types.hpp>

#include <lama/pcap/include/pcap.hpp>


namespace lama {
namespace pcap {

    struct stats
    {
        msw::uint32 received          ;
        msw::uint32 dropped           ;
        msw::uint32 interface_dropped ;
#ifdef HAVE_REMOTE
        msw::uint32 captured          ;
        msw::uint32 sent              ;
        msw::uint32 net_dropped       ;
#endif
    };

    static_assert(sizeof(stats) == sizeof(pcap_stat), "size of 'stats' does not match size of 'pcap_stat'");

}}
