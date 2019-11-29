#pragma once

#include <msw/config.hpp>

#include <lama/pcap/handle.hpp>


namespace lama {
namespace pcap {

    int const default_read_timeout = 100   ;
    int const default_snaplen      = 65536 ;
    int const default_open_flags   =
#ifdef MSW_WINDOWS
        PCAP_OPENFLAG_PROMISCUOUS
#else
        1
#endif
        ;

}}
