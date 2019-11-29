#pragma once

#include <msw/include/boost/asio.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <dawn/net/port_pair.hpp>


namespace dawn {
namespace net  {

    template <typename Protocol>
    struct endpoint_pair
    {
        typedef Protocol                     protocol_type ;
        typedef endpoint_pair<protocol_type> self_type     ;

        boost::asio::ip::basic_endpoint<Protocol> source      ;
        boost::asio::ip::basic_endpoint<Protocol> destination ;

        bool operator < (self_type const& o) const
        {
            return source == o.source ? destination < o.destination : source < o.source;
        }

        self_type reverse() const
        {
            return self_type{destination, source};
        }
        self_type forward() const
        {
            return source.port() < destination.port() ? *this : reverse();
        }

        std::string string() const
        {
            return msw::network_endpoint_pair_to_string(source, destination);
        }

        port_pair make_port_pair() const
        {
            port_pair ports;
            ports.source      = source     .port();
            ports.destination = destination.port();
            return ports;
        }
    };

    typedef endpoint_pair<boost::asio::ip::tcp> tcp_endpoint_pair ;
    typedef endpoint_pair<boost::asio::ip::udp> udp_endpoint_pair ;

}}
