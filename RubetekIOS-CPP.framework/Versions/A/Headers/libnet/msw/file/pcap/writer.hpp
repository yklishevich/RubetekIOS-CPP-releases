#pragma once

#include <msw/plain_types.hpp>           // byte
#include <msw/noncopyable.hpp>
#include <msw/file/optimum_ofile2.hpp>
#include <msw/file/pcap/headers.hpp>
#include <msw/make_range/from_plain.hpp>


namespace msw       {
namespace pcap_file {

    struct writer
        : noncopyable
    {
        explicit writer       (std::string const& file_name, qbyte linktype = pcap_header::default_link_type)       ;
        void     write_packet (range<byte const>, ebyte time_marks = 0)                                             ;
        uint64   size         ()                                                                              const ;
    private:
        optimum_ofile2 ofile_;
    };

}}


namespace msw       {
namespace pcap_file {

    inline writer::writer(std::string const& file_name, qbyte linktype)
        : ofile_(file_name)
    {
        pcap_header pcap_header_;
        pcap_header_.default_initialize();
        pcap_header_.linktype = linktype;
        ofile_.write(make_range<byte>(pcap_header_));
    }
    inline void writer::write_packet(range<byte const> payload, ebyte time_marks)
    {
        packet_header packet_header_;
        packet_header_.time_marks = time_marks;
        packet_header_.set_size(static_cast<qbyte>(payload.size().count()));
        ofile_.write(make_range<byte>(packet_header_));
        ofile_.write(payload);
    }
    inline uint64 writer::size() const
    {
        return ofile_.size();
    }

}}
