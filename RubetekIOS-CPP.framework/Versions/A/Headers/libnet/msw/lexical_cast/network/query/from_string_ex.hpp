#pragma once 

#include <string>

#include <msw/std/regex.hpp>
#include <msw/plain_types.hpp>                            // wbyte
#include <msw/include/boost/asio.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/network/query/from_string.hpp>


namespace msw
{
    template <typename Protocol>
    boost::asio::ip::basic_resolver_query<Protocol> network_query_from_string_ex(std::string const& host, std::string const& default_port)
    {
        static regex const expr("(.*?)(?::(.*))?");
        smatch what;
        if (regex_match(host, what, expr))
        {
            std::string const port = what[2];
            return network_query_from_string<Protocol>(what[1], (port.empty() ? default_port : port));
        }
        throw_runtime_error("can't parse [host:port]: ", host);
#include <msw/throw_hide_warning.hpp>
    }
    template <typename Protocol>
    boost::asio::ip::basic_resolver_query<Protocol> network_query_from_string_ex(std::string const& host, wbyte port)
    {
        return network_query_from_string_ex<Protocol>(host, std::to_string(port));
    }
    template <typename Protocol>
    boost::asio::ip::basic_resolver_query<Protocol> network_query_from_string_ex(std::string const& host, int port)
    {
        return network_query_from_string_ex<Protocol>(host, static_cast<wbyte>(port));
    }

    template <typename ...Args>
    boost::asio::ip::basic_resolver_query<boost::asio::ip::udp> udp_query_from_string_ex(Args ...args)
    {
        return network_query_from_string_ex<boost::asio::ip::udp>(args...);
    }
    template <typename ...Args>
    boost::asio::ip::basic_resolver_query<boost::asio::ip::tcp> tcp_query_from_string_ex(Args ...args)
    {
        return network_query_from_string_ex<boost::asio::ip::tcp>(args...);
    }
}
