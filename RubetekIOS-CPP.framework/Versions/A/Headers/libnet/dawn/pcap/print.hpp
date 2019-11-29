#pragma once

#include <msw/cwrite.hpp>

#include <msw/pcap/find_devices.hpp>
#include <msw/pcap/correct_device_name.hpp>
#include <msw/pcap/correct_device_description.hpp>


namespace dawn {
namespace pcap {

    inline void print_adapters(std::vector<msw::pcap::device_info> const& devices)
    {
        if (devices.empty()) msw::cwriteln("adapters not found");
        else
        {
            int n = 0;
            for (msw::pcap::device_info const& info : devices)
                msw::cwriteln(n++, ". ", msw::pcap::correct_device_name(info.name), ": ", msw::pcap::correct_device_description(info.description));
        }
    }

    inline void print_adapters()
    {
        print_adapters(msw::pcap::find_devices());
    }

}}
