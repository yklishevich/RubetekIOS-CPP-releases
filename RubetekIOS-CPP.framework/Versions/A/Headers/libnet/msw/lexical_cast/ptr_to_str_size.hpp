#pragma once 

#include <msw/int.hpp>
#include <msw/ptr.hpp>
#include <msw/size.hpp>
#include <msw/bit_ptr.hpp>
#include <msw/lexical_cast/int_to/hex_size.hpp>


namespace msw
{
    template <typename T>
    size<char> ptr_to_str_size()
    {
        return int_to_hex_size<typename T::number_type>();
    }
    template <>
    inline size<char> ptr_to_str_size<bit_ptr>()
    {
        return int_to_hex_size<uint>() + 2;
    }
    template <>
    inline size<char> ptr_to_str_size<bit_const_ptr>()
    {
        return ptr_to_str_size<bit_ptr>();
    }
}
