#pragma once 

#include <string>

#include <msw/lexical_cast/network/endpoint/to_string.hpp>


namespace msw
{
    template <typename Protocol>
    std::string network_query_to_string(boost::asio::ip::basic_resolver_query<Protocol> const& resolver_query)
    {
        return network_endpoint_to_string(resolver_query.host_name(), resolver_query.service_name());
    }
}
