#pragma once

#include <string>

#include <msw/config.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>


namespace dawn {
namespace net  {

    template <typename T>
    std::string remote_endpoint_to_string(T const* o)
    {
        boost::system::error_code ec;
        auto endpoint = o->remote_endpoint(ec);
        return ec ? std::string("no-remote-endpoint") : msw::network_endpoint_to_string(endpoint);
    }

}}
