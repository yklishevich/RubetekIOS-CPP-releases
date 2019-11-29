#pragma once

#include <cassert>

#include <boost/iterator/iterator_traits.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace dawn             {
namespace percent_encoding {
namespace zzz              {

    template <typename CharT>
    CharT letter_to_hex(CharT in)
    {
        switch (in)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return in - '0';
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
                return in + 10 - 'a';
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
                return in + 10 - 'A';
        }
        return CharT();
    }

    template <class InputIterator, class OutputIterator>
    OutputIterator decode_impl
    (
          InputIterator  const& in_begin
        , InputIterator  const& in_end
        , OutputIterator const& out_begin
    )
    {
        typedef typename boost::iterator_value<InputIterator>::type value_type;

        InputIterator it = in_begin;
        OutputIterator out = out_begin;
        while (it != in_end)
        {
            if (*it == '%')
            {
                ++it;
                value_type v0 = letter_to_hex(*it);
                ++it;
                value_type v1 = letter_to_hex(*it);
                ++it;
                *out++ = 0x10 * v0 + v1;
            }
            else if (*it == '+')
            {
                *out++ = ' ';
                ++it;
            }
            else
            {
                *out++ = *it++;
            }
        }
        return out;
    }

}}}


namespace dawn             {
namespace percent_encoding {

    template <class SinglePassRange, class OutputIterator>
    OutputIterator decode(SinglePassRange const& range, OutputIterator const& out)
    {
        return zzz::decode_impl(boost::begin(range), boost::end(range), out);
    }
    inline std::string decode(std::string const& input)
    {
        std::string decoded;
        decode(input, std::back_inserter(decoded));
        return decoded;
    }

}}
