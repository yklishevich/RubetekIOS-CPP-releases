#pragma once

#include <msw/size.hpp>
#include <msw/plain_traits.hpp> // byte, wbyte, qbyte


namespace msw   {
namespace train {

    static size<byte> const default_packet_size = KB * 8  ;
    static size<byte> const min_packet_size     = 64      ;
    static size<byte> const max_packet_size     = KB * 64 ;
    static size<byte> const wagon_max_size      = 0xffff  ;

    struct packet_header
    {
        wbyte counter ;
        wbyte pointer ;
    };
    static_assert(sizeof(packet_header) == 4, "size of packet header must be 4 bytes");

    struct wagon_header
    {
        wbyte len ;
        wbyte ext ;
        qbyte pid ;
        qbyte lid ;
    };
    static_assert(sizeof(wagon_header) == 12, "size of wagon header must be 12 bytes");

}}
