#pragma once

#include <msw/plain_types.hpp>     // byte
#include <msw/zzz/bit_ptr/def.hpp>


namespace msw
{
    typedef bit_ptr_t<byte>       bit_ptr       ;
    typedef bit_ptr_t<byte const> bit_const_ptr ;
}
