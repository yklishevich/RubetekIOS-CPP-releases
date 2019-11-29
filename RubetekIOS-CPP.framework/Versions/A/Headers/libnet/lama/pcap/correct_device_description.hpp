#pragma once

#include <string>

#include <msw/config.hpp>
#ifdef MSW_WINDOWS
#   include <msw/std/regex.hpp>
#endif


namespace lama {
namespace pcap {

    inline std::string correct_device_description(std::string const& s)
    {
#ifdef MSW_WINDOWS
        static msw::regex const e("'(.*?) *'");
        msw::smatch w;
        if (msw::regex_search(s, w, e)) return w[1];
#endif
        return s;
    }

}}
