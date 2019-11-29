#pragma once

#include <string>
#include <memory>

#include <msw/config.hpp>
#include <msw/std/memory.hpp>
#include <msw/throw_runtime_error.hpp>

#include <lama/pcap/handle.hpp>
#include <lama/pcap/default.hpp>
#include <lama/pcap/include/pcap.hpp>


namespace lama {
namespace pcap {

    inline handle open
    (
          std::string const&            name
        , std::unique_ptr<std::string>& error
        , int                           read_timeout = default_read_timeout
        , int                           snaplen      = default_snaplen
        , int                           flags        = default_open_flags
#ifdef MSW_WINDOWS
        , pcap_rmtauth*                 auth         = nullptr
#endif
    )
    {
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t* h =
#ifdef MSW_WINDOWS
            ::pcap_open(name.c_str(), snaplen, flags, read_timeout, auth, errbuf);
#else
            ::pcap_open_live(name.c_str(), snaplen, flags, read_timeout, errbuf);
#endif
        if (!h)
        {
            error = msw::make_unique<std::string>(errbuf);
            return handle();
        }
        return handle(h);
    }

    inline handle open
    (
          std::string const& name
        , int                read_timeout = default_read_timeout
        , int                snaplen      = default_snaplen
        , int                flags        = default_open_flags
#ifdef MSW_WINDOWS
        , pcap_rmtauth*      auth         = nullptr
#endif
    )
    {
        std::unique_ptr<std::string> error;
        handle h = open
        (
              name
            , error
            , read_timeout
            , snaplen
            , flags
#ifdef MSW_WINDOWS
            , auth
#endif
        );
        if (!h)
        {
            MSW_ASSERT(error);
            msw::throw_runtime_error("unable to open adapter '", name, "': ", *error);
        }
        return std::move(h);
    }

}}
