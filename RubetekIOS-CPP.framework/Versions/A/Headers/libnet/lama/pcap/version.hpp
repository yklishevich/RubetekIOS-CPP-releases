#pragma once

#include <string>

#include <msw/config.hpp>
#ifdef MSW_WINDOWS
#   include <msw/format.hpp>
#   include <msw/std/regex.hpp>
#endif
#include <lama/pcap/include/pcap.hpp>


namespace lama {
namespace pcap {

    struct version
    {
        static std::string original()
        {
            return get_impl().original;
        }
        static std::string const& winpcap()
        {
            return get_impl().winpcap;
        }
        static std::string const& packet_dll()
        {
            return get_impl().packet_dll;
        }
        static std::string const& libpcap()
        {
            return get_impl().libpcap;
        }
        static std::string const& full()
        {
            return get_impl().full;
        }
    private:

        struct impl
        {
            impl()
                : original(::pcap_lib_version())
            {
#ifdef MSW_WINDOWS
                msw::regex e("WinPcap version (.*?) \\(packet\\.dll version (.*?)\\).*?version (.*?) branch (.*?) \\((.*?)\\)");
                msw::smatch w;
                if (msw::regex_match(original, w, e))
                {
                    winpcap = w[1];
                    packet_dll = w[2];
                    for (int i = 3; i != 6; ++i)
                    {
                        if (!libpcap.empty()) libpcap.push_back('-');
                        libpcap.append(w[i]);
                    }
                }
                full = msw::format(winpcap, "(packet.dll-", packet_dll, "):libpcap-", libpcap);
#endif
            }

            std::string original   ;
            std::string winpcap    ;
            std::string packet_dll ;
            std::string libpcap    ;
            std::string full       ;
        };

        static impl const& get_impl()
        {
            static impl const impl_;
            return impl_;
        }
    };

}}
