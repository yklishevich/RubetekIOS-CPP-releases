#pragma once 

#include <string>

#include <msw/lexical_cast/network/query/to_string.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>


namespace msw
{
    template <typename Protocol>
    std::string network_point_to_string(boost::asio::ip::basic_resolver_query<Protocol> const& query)
    {
        return network_query_to_string(query);
    }
    template <typename Protocol>
    std::string network_point_to_string(boost::asio::ip::basic_endpoint<Protocol> const& endpoint)
    {
        return network_endpoint_to_string(endpoint);
    }
}
