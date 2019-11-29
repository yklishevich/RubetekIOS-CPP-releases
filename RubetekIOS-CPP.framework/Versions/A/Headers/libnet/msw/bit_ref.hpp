#pragma once

#include <msw/plain_types.hpp>          // byte
#include <msw/zzz/bit_ref/def.hpp>
#include <msw/zzz/bit_ref/one_zero.hpp>


namespace msw
{
    typedef bit_ref_t<byte>       bit_ref       ;
    typedef bit_ref_t<byte const> bit_const_ref ;

    static bit_const_ref const one_bit  = bit_const_ref(zzz::one_bit_impl, 0)  ;
    static bit_const_ref const zero_bit = bit_const_ref(zzz::zero_bit_impl, 0) ;
}
