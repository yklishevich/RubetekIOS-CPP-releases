#pragma once

#include <msw/config.hpp>
#include <msw/plain_types.hpp>
#include <msw/include/boost/asio.hpp>


namespace dawn {
namespace net  {

    template <typename Protocol>
    boost::asio::ip::basic_endpoint<Protocol> make_endpoint(boost::asio::ip::address ip, msw::wbyte port)
    {
        return boost::asio::ip::basic_endpoint<Protocol>(ip, port);
    }

    inline boost::asio::ip::basic_endpoint<boost::asio::ip::udp> make_udp_endpoint(boost::asio::ip::address ip, msw::wbyte port)
    {
        return make_endpoint<boost::asio::ip::udp>(ip, port);
    }
    inline boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> make_tcp_endpoint(boost::asio::ip::address ip, msw::wbyte port)
    {
        return make_endpoint<boost::asio::ip::tcp>(ip, port);
    }

}}
