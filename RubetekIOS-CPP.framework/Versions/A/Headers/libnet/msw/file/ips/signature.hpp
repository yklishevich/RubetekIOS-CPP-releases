#pragma once

#include <string>

#include <msw/buffer.hpp>
#include <msw/make_range.hpp>


namespace msw {
namespace ips {

    inline range<byte const> signature()
    {
        static buffer<byte> const sign(make_range<byte>(std::string("IP_STREAM")));
        return sign.all();
    }

}}
