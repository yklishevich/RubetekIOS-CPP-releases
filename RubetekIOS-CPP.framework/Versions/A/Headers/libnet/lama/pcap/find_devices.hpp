#pragma once

#include <vector>

#include <msw/config.hpp>
#include <msw/scoped_shut.hpp>
#include <msw/throw_runtime_error.hpp>

#include <dawn/net/ip_address_from_sockaddr.hpp>

#include <lama/pcap/device_info.hpp>
#include <lama/pcap/include/pcap.hpp>


namespace lama {
namespace pcap {

    inline std::list<boost::asio::ip::address> make_addresses(pcap_addr* addr)
    {
        std::list<boost::asio::ip::address> addresses;
        for (pcap_addr* a = addr; a; a = a->next)
        {
            boost::optional<boost::asio::ip::address> ip = dawn::net::ip_address_from_sockaddr(a->addr);
            if (ip) addresses.push_back(*ip);
        }
        return addresses;
    }

    inline std::vector<device_info> find_devices()
    {
        pcap_if_t* alldevs;
        char errbuf[PCAP_ERRBUF_SIZE];
        if (
#ifdef MSW_WINDOWS
            ::pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf)
#else
            ::pcap_findalldevs(&alldevs, errbuf)
#endif
            == -1)
            msw::throw_runtime_error("pcap find device failed: ", errbuf);
        msw::scoped_shut scoped_shut
        (
            [&]()
            {
                ::pcap_freealldevs(alldevs);
            }
        );
        std::vector<device_info> devices;
        for(pcap_if_t* dev = alldevs; dev; dev = dev->next)
            devices.push_back
            ({
                  dev->name
#ifdef MSW_WINDOWS
                , dev->description
#else
                , ""
#endif
                , make_addresses(dev->addresses)
                , dev->flags
            });
        return std::move(devices);
    }

}}
