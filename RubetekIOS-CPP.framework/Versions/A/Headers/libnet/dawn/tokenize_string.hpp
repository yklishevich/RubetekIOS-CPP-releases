#pragma once

#include <list>
#include <string>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>


namespace dawn
{
    inline std::list<std::string> tokenize_string(std::string const& s, char const separator)
    {
        std::list<std::string> lst;
        typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
        std::string ss;
        ss.push_back(separator);
        boost::char_separator<char> sep(ss.data());
        tokenizer tok(s, sep);
        for(tokenizer::iterator i = tok.begin(); i != tok.end(); ++i)
            lst.push_back(*i);
        return lst;
    }
    template <typename T>
    std::list<T> tokenize_string_cast(std::string const& s, char const separator)
    {
        auto strings = tokenize_string(s, separator);
        std::list<T> lst;
        for (std::string const& x : strings)
            lst.push_back(boost::lexical_cast<T>(x));
        return lst;
    }
}
