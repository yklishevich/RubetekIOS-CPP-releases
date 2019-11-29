#pragma once

#include <list>
#include <string>

#include <msw/plain_types.hpp>
#include <msw/include/boost/asio.hpp>


namespace lama {
namespace pcap {

    struct device_info
    {
        std::string                         name        ;
        std::string                         description ;
        std::list<boost::asio::ip::address> addresses   ;
        msw::uint32                         flags       ;
    };

}}
