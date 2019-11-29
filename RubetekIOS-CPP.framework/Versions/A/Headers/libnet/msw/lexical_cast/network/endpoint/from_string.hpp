#pragma once

#include <regex>

#include <boost/lexical_cast.hpp>

#include <msw/config.hpp>
#include <msw/std/regex.hpp>
#include <msw/plain_types.hpp>         // wbyte
#include <msw/include/boost/asio.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw
{
    template <typename Protocol>
    boost::asio::ip::basic_endpoint<Protocol> network_endpoint_from_string(std::string const& host, wbyte port)
    {
        return boost::asio::ip::basic_endpoint<Protocol>(boost::asio::ip::address::from_string(host), port);
    }
    template <typename Protocol>
    boost::asio::ip::basic_endpoint<Protocol> network_endpoint_from_string(std::string const& host, int port)
    {
        return network_endpoint_from_string<Protocol>(host, static_cast<wbyte>(port));
    }
    template <typename Protocol>
    boost::asio::ip::basic_endpoint<Protocol> network_endpoint_from_string(std::string const& host, std::string const& port)
    {
        return boost::asio::ip::basic_endpoint<Protocol>(boost::asio::ip::address::from_string(host), boost::lexical_cast<wbyte>(port));
    }
    template <typename Protocol>
    boost::asio::ip::basic_endpoint<Protocol> network_endpoint_from_string(std::string const& s)
    {
        smatch match;
        if (regex_match(s, match, regex("(.*?):(\\d+)")))
            return network_endpoint_from_string<Protocol>(match[1], match[2]);
        throw_runtime_error("can't parse [host:port]: ", s);
#ifdef MSW_MSVC
        MSW_ON_DEBUG(throw);
#endif
    }

    template <typename ...Args>
    boost::asio::ip::basic_endpoint<boost::asio::ip::udp> udp_endpoint_from_string(Args ...args)
    {
        return network_endpoint_from_string<boost::asio::ip::udp>(args...);
    }
    template <typename ...Args>
    boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> tcp_endpoint_from_string(Args ...args)
    {
        return network_endpoint_from_string<boost::asio::ip::tcp>(args...);
    }
}
