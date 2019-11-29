#pragma once 

#include <string>

#include <msw/config.hpp>
#include <msw/plain_types.hpp>        // wbyte
#include <msw/include/boost/asio.hpp>


namespace msw
{
    inline std::string network_endpoint_to_string(std::string const& host, std::string const& port)
    {
        return host + ":" + port;
    }
    inline std::string network_endpoint_to_string(wbyte port)
    {
        return
#ifdef MSW_NO_COMPLETE_TO_STRING
            std::to_string(static_cast<long long int>(port))
#else
            std::to_string(port)
#endif
        ;
    }
    inline std::string network_endpoint_to_string(std::string const& host, wbyte port)
    {
        return network_endpoint_to_string(host, network_endpoint_to_string(port));
    }
    template <typename Port>
    inline std::string network_endpoint_to_string(boost::asio::ip::address address, Port&& port)
    {
        return network_endpoint_to_string(address.to_string(), std::forward<Port>(port));
    }
    template <typename Protocol>
    std::string network_endpoint_to_string(boost::asio::ip::basic_endpoint<Protocol> const& endpoint)
    {
        return network_endpoint_to_string(endpoint.address().to_string(), endpoint.port());
    }
    template <typename Protocol>
    std::string network_endpoint_pair_to_string
    (
          boost::asio::ip::basic_endpoint<Protocol> const& src_endpoint
        , boost::asio::ip::basic_endpoint<Protocol> const& dst_endpoint
    )
    {
        return network_endpoint_to_string(src_endpoint) + "->" + network_endpoint_to_string(dst_endpoint);
    }
    template <typename Address, typename Port>
    std::string network_endpoint_pair_to_string(Address&& src_address, Port&& src_port, Address&& dst_address, Port&& dst_port)
    {
        return network_endpoint_to_string(std::forward<Address>(src_address), std::forward<Port>(src_port)) + "->"
            + network_endpoint_to_string(std::forward<Address>(dst_address), std::forward<Port>(dst_port));
    }
}
