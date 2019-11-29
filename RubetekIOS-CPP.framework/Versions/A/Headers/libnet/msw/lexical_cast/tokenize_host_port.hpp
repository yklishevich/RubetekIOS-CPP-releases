#pragma once 

#include <regex>
#include <string>

#include <msw/plain_types.hpp>    // wbyte

#include <boost/lexical_cast.hpp>


namespace msw
{
    template <typename Fn>
    inline void tokenize_host_port(std::string const& s, Fn fn)
    {
        std::regex const expr("(.*?):(.*)");
        std::regex const expr_port("(?:(\\d+)-(\\d+))|(\\d+)");
        std::smatch what;
        if (std::regex_match(s, what, expr))
        {
            std::string const host = what[1];
            std::smatch what_port;
            std::string::const_iterator pos = what[2].first;
            std::string::const_iterator const end = what[2].second;
            while (std::regex_search(pos, end, what_port, expr_port))
            {
                if (what_port[3] == "")
                {
                    wbyte start_port = boost::lexical_cast<unsigned short>(what_port[1]);
                    wbyte end_port = boost::lexical_cast<unsigned short>(what_port[2]);
                    if (start_port > end_port) throw std::runtime_error("incorrect port range");
                    for (wbyte port = start_port; port <= end_port; ++port) fn(host, port);
                }
                else fn(host, boost::lexical_cast<unsigned short>(what_port[3]));
                pos = what_port[0].second;
            }
        }
        else throw std::runtime_error("can't parse host:port");
    }
}
