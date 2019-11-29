#pragma once

#include <msw/config.hpp>
#include <msw/plain_types.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/lexical_cast/network/query/from_string.hpp>


namespace dawn {
namespace net  {

    template <typename Protocol>
    typename boost::asio::ip::basic_resolver<Protocol>::query endpoint_to_query(boost::asio::ip::basic_endpoint<Protocol> endpoint)
    {
        return msw::network_query_from_string<Protocol>(endpoint.address().to_string(), endpoint.port());
    }

    inline boost::asio::ip::basic_resolver_query<boost::asio::ip::udp> udp_endpoint_to_query(boost::asio::ip::basic_endpoint<boost::asio::ip::udp> endpoint)
    {
        return endpoint_to_query<boost::asio::ip::udp>(endpoint);
    }
    inline boost::asio::ip::basic_resolver_query<boost::asio::ip::tcp> tcp_endpoint_to_query(boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> endpoint)
    {
        return endpoint_to_query<boost::asio::ip::tcp>(endpoint);
    }

}}
