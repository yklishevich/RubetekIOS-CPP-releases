#pragma once 

#include <string>

#include <msw/ipow.hpp>
#include <msw/std/regex.hpp>
#include <msw/lexical_cast/int_from/dec.hpp>


namespace msw {
namespace zzz {

    template <typename T, int N = 1000>
    T count_from_string(std::string const& s)
    {
        msw::regex e("(\\d+) ?(K|M|G|T)?");
        msw::smatch m;
        if (!msw::regex_match(s, m, e)) throw std::runtime_error("can't convert size from string. bad format");
        int p = 0;
        if (m[2] == "K") p = 1;
        else if (m[2] == "M") p = 2;
        else if (m[2] == "G") p = 3;
        else if (m[2] == "T") p = 4;
        return int_from_dec<T>(m[1]) * ipow<T>(N, p);
    }

}}
