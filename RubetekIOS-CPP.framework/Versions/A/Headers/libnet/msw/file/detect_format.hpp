#pragma once

#include <string>

#include <msw/file_path.hpp>
#include <msw/plain_types.hpp>              // byte
#include <msw/file/format.hpp>
#include <msw/file/pcap/headers.hpp>
#include <msw/file/ips/signature.hpp>
#include <msw/zzz/file/check_signature.hpp>


namespace msw  {
namespace file {

    inline format detect_format(std::string const& file_name)
    {
        if (zzz::file::check_signature(file_name, ips::signature()))                          return format::ips  ;
        if (zzz::file::check_signature(file_name, make_range<byte>(pcap_file::magic_number))) return format::pcap ;
        return format::bin;
    }

    inline format detect_format_by_ext(std::string const& file_name)
    {
        std::string const ext = file_path::extension(file_name);
        if      (ext == "ips"   ) return format::ips    ;
        else if (ext == "pcap"  ) return format::pcap   ;
        else if (ext == "pcapng") return format::pcapng ;
        return format::bin;
    }

}}
