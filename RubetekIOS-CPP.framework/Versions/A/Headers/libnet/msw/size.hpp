#pragma once

#include <msw/plain_types.hpp>  // bit, byte
#include <msw/zzz/size/def.hpp>


namespace msw
{
    typedef size<bit> bit_size;

    static size<byte> const KB =      1024 ;
    static size<byte> const MB = KB * 1024 ;
    static size<byte> const GB = MB * 1024 ;
    static size<byte> const TB = GB * 1024 ;
}
