#pragma once

#include <functional>

#include <msw/range.hpp>
#include <msw/plain_types.hpp> // byte


namespace msw      {
namespace callback {

    typedef std::function<void(range<byte>)>       byte_range       ;
    typedef std::function<void(range<byte const>)> byte_const_range ;

}}
