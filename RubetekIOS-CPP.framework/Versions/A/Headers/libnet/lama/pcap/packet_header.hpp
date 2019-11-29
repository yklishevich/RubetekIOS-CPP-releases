#pragma once

#include <string>

#include <msw/plain_types.hpp>

#include <lama/pcap/include/pcap.hpp>


namespace lama {
namespace pcap {

    struct packet_header
    {
        timeval     ts     ;
        msw::uint32 caplen ;
        msw::uint32 len    ;
    };

    static_assert(sizeof(packet_header) == sizeof(pcap_pkthdr), "size of 'packet_header' does not match size of 'pcap_pkthdr'");

}}
