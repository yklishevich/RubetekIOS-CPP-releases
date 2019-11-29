#pragma once


#include <msw/size.hpp>
#include <msw/plain_types.hpp>  // wbyte, qbyte, ebyte

namespace msw       {
namespace pcap_file {

    static const qbyte magic_number = 0xa1b2c3d4;

#pragma pack(push, 1)
    struct pcap_header
    {
        qbyte magic_number  ;
        wbyte version_major ;
        wbyte version_minor ;
        qbyte thiszone      ;
        qbyte sigfigs       ;
        qbyte snaplen       ;
        qbyte linktype      ;

        static qbyte const default_link_type = 1;

        pcap_header& default_initialize()
        {
            magic_number  = pcap_file::magic_number;
            version_major = 2;
            version_minor = 4;
            thiszone      = 0;
            sigfigs       = 0;
            snaplen       = static_cast<qbyte>((KB * 64).count());
            linktype      = default_link_type;
            return *this;
        }

        bool check_magic_number() const
        {
            return magic_number == pcap_file::magic_number;
        }
    };

    struct packet_header
    {
        ebyte time_marks    ;
        qbyte capture_size  ;
        qbyte original_size ;

        packet_header& default_initialize()
        {
            time_marks    = 0;
            capture_size  = 0;
            original_size = 0;
            return *this;
        }

        void set_size(qbyte size)
        {
            capture_size  = size;
            original_size = size;
        }
    };
#pragma pack(pop)

}}
