#pragma once

#include <string>
#include <memory>

#include <msw/range.hpp>
#include <msw/format.hpp>
#include <msw/std/memory.hpp>
#include <msw/make_range_native.hpp>
#include <msw/throw_runtime_error.hpp>

#include <lama/pcap/handle.hpp>
#include <lama/pcap/include/pcap.hpp>
#include <lama/pcap/packet_header.hpp>


namespace lama {
namespace pcap {

    inline msw::range<msw::byte const> next_nothrow(handle const& h, std::unique_ptr<std::string>& error, packet_header* header = nullptr)
    {
        pcap_pkthdr* hdr;
        msw::byte const* pkt_data;
        msw::range<msw::byte const> packet;
        int const res = ::pcap_next_ex(h.native(), &hdr, &pkt_data);
        if (res == -1) error = msw::make_unique<std::string>(msw::format("pcap next: ", static_cast<char const*>(::pcap_geterr(h.native()))));
        else if (res == -2) error = msw::make_unique<std::string>(msw::format("pcap next: eof was reached reading from an offline capture"));
        else if (res == 1)
        {
            packet = msw::make_range_native(pkt_data, hdr->caplen);
            if (header) *header = *reinterpret_cast<packet_header*>(hdr);
        }
        else if (res != 0) error = msw::make_unique<std::string>(msw::format("pcap_next_ex return invalid value: ", res));
        return packet;
    }
    inline msw::range<msw::byte const> next_nothrow(handle const& h, std::unique_ptr<std::string>& error, packet_header& header)
    {
        return next_nothrow(h, error, &header);
    }
    inline msw::range<msw::byte const> next(handle const& h, packet_header* header = nullptr)
    {
        std::unique_ptr<std::string> error;
        msw::range<msw::byte const> packet = next_nothrow(h, error, header);
        if (error) msw::throw_runtime_error(*error);
        return packet;
    }
    inline msw::range<msw::byte const> next(handle const& h, packet_header& header)
    {
        return next(h, &header);
    }

}}
