#pragma once

#include <msw/size.hpp>
#include <msw/assert.hpp>
#include <msw/plain_types.hpp>        // byte
#include <msw/noncopyable.hpp>
#include <msw/file/optimum_ifile.hpp>
#include <msw/file/pcap/headers.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw       {
namespace pcap_file {

    struct reader
        : noncopyable
    {
        explicit           reader           (
                                                  std::string const& file_name
                                                , size<byte> = MSW_IFILE_DEFAULT_BLOCK_SIZE
                                            )        ;

        pcap_header const& header           () const ;

        bool               has_packets      () const ;
        size<byte>         packet_size      () const ;
        ebyte              time_marks       () const ;
        range<byte>        read_packet      ()       ;

        uint64             readed_raw_bytes () const ;

    private:

        void read_packet_header();

        optimum_ifile ifile_         ;
        pcap_header   header_        ;
        bool          has_packets_   ;
        packet_header packet_header_ ;
    };

}}


namespace msw       {
namespace pcap_file {

    template <typename Handler>
    void read(std::string const& file_name, Handler);

}}


namespace msw       {
namespace pcap_file {

    inline reader::reader(std::string const& file_name, size<byte> block_size)
        : ifile_ (file_name, block_size)
    {
        range<byte const> header = ifile_.read(size<pcap_header>(1));
        if (header.size() != size<pcap_header>(1)) throw_runtime_error("invalid pcap header size");
        header_ = header.as<pcap_header>();
        if (!header_.check_magic_number()) throw_runtime_error("invalid pcap header magic number");
        read_packet_header();
    }

    inline pcap_header const& reader::header() const
    {
        return header_;
    }

    inline bool reader::has_packets() const
    {
        return has_packets_;
    }
    inline size<byte> reader::packet_size() const
    {
        MSW_ASSERT(has_packets());
        return packet_header_.capture_size;
    }
    inline ebyte reader::time_marks() const
    {
        MSW_ASSERT(has_packets());
        return packet_header_.time_marks;
    }
    inline range<byte> reader::read_packet()
    {
        MSW_ASSERT(has_packets());
        if (packet_size() > MB) throw_runtime_error("very large packet size (> MB): ", packet_size());
        range<byte> packet = ifile_.read_imperative(packet_size());
        read_packet_header();
        return packet;
    }

    inline uint64 reader::readed_raw_bytes() const
    {
        return ifile_.position();
    }

    inline void reader::read_packet_header()
    {
        range<byte const> r = ifile_.read(sizeof(packet_header));
        has_packets_ = (r.size() == size<packet_header>(1));
        if (has_packets_) packet_header_ = r.as<packet_header>();
        else              packet_header_.default_initialize();
    }

}}


namespace msw       {
namespace pcap_file {

    template <typename Handler>
    void read(std::string const& file_name, Handler h)
    {
        reader reader_(file_name);
        while (reader_.has_packets()) h(reader_.read_packet(), reader_.time_marks());
    }

}}
