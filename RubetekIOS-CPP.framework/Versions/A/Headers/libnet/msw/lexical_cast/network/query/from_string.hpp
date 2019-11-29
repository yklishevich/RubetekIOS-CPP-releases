#pragma once 

#include <string>

#include <msw/on_debug.hpp>
#include <msw/std/regex.hpp>
#include <msw/plain_types.hpp>         // wbyte
#include <msw/include/boost/asio.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw
{
    template <typename Protocol>
    boost::asio::ip::basic_resolver_query<Protocol> network_query_from_string(std::string const& host, std::string const& port)
    {
        return boost::asio::ip::basic_resolver_query<Protocol>(host, port);
    }
    template <typename Protocol>
    boost::asio::ip::basic_resolver_query<Protocol> network_query_from_string(std::string const& host, wbyte port)
    {
        return network_query_from_string<Protocol>(host, std::to_string(port));
    }
    template <typename Protocol>
    boost::asio::ip::basic_resolver_query<Protocol> network_query_from_string(std::string const& host, int port)
    {
        return network_query_from_string<Protocol>(host, static_cast<wbyte>(port));
    }
    template <typename Protocol>
    boost::asio::ip::basic_resolver_query<Protocol> network_query_from_string(std::string const& s)
    {
        smatch match;
        if (regex_match(s, match, regex("(.*?):(.*)")))
            return network_query_from_string<Protocol>(match[1], match[2]);
        throw_runtime_error("can't parse [host:port]: ", s);
        MSW_ON_DEBUG(throw);
    }

    template <typename ...Args>
    boost::asio::ip::basic_resolver_query<boost::asio::ip::udp> udp_query_from_string(Args ...args)
    {
        return network_query_from_string<boost::asio::ip::udp>(args...);
    }
    template <typename ...Args>
    boost::asio::ip::basic_resolver_query<boost::asio::ip::tcp> tcp_query_from_string(Args ...args)
    {
        return network_query_from_string<boost::asio::ip::tcp>(args...);
    }
}
