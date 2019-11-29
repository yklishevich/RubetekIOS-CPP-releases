#pragma once

#include <string>


namespace msw  {
namespace file {

    enum class format
    {
          bin
        , ips
        , pcap
        , pcapng
    };

    inline std::string format_to_string(format fmt)
    {
        if      (fmt == format::ips    ) return "ips"    ;
        else if (fmt == format::pcap   ) return "pcap"   ;
        else if (fmt == format::pcapng ) return "pcapng" ;
        return "bin";
    }

}}
