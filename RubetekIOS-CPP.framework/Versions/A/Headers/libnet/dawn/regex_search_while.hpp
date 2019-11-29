#pragma once

#include <string>
#include <regex>

#include <msw/std/regex.hpp>


namespace dawn
{
    template <class Iterator, class charT, class Handler, class traits>
    bool regex_search_while(Iterator first, Iterator last, Handler handler, msw::basic_regex<charT, traits> const& rgx)
    {
        bool result = false;
        msw::match_results<Iterator> m;
        while (msw::regex_search(first, last, m, rgx))
        {
            result = true;
            first = m[0].second;
            if (!handler(m)) break;
        }
        return result;
    }
    template <typename Handler, typename Regex>
    bool regex_search_while(std::string const& s, Handler&& handler, Regex const& rgx)
    {
        return regex_search_while(s.begin(), s.end(), std::forward<Handler>(handler), rgx);
    }
}
