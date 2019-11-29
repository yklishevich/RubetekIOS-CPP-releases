#pragma once

#include <string>

#include <msw/throw_runtime_error.hpp>

#include <lama/pcap/device_info.hpp>


namespace dawn {
namespace pcap {

    inline int find_device(std::string const& name, std::vector<lama::pcap::device_info> const& devices, bool& error)
    {
        int n = 0;
        for (lama::pcap::device_info const& i : devices)
        {
            if ((i.name.find(name) != std::string::npos) || (i.description.find(name) != std::string::npos)) return n;
            n++;
        }
        error = true;
        return -1;
    }

    inline int find_device(std::string const& name, std::vector<lama::pcap::device_info> const& devices)
    {
        bool error = false;
        int const n = find_device(name, devices, error);
        if (error) msw::throw_runtime_error("device '", name, "' not found in pcap devices");
        return n;
    }

}}
