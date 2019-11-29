#pragma once

#include <lama/pcap/version.hpp>


namespace msw
{
    static std::string const& version_of_pcap()
    {
        return lama::pcap::version::winpcap();
    }
}
