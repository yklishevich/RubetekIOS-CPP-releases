#pragma once

#include <msw/plain_types.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>


namespace dawn {
namespace net  {

    struct universal_endpoint
    {
        boost::asio::ip::address address ;
        msw::wbyte               port    ;

        std::string string() const
        {
            return msw::network_endpoint_to_string(address, port);
        }
    };

}}


namespace dawn {
namespace net  {

    struct universal_endpoint_pair
    {
        universal_endpoint source      ;
        universal_endpoint destination ;

        std::string string() const
        {
            return source.string() + "->" + destination.string();
        }
    };

}}
