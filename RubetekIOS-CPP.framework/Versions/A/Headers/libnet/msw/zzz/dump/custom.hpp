#pragma once

#include <string>
#include <type_traits>                       // std::enable_if

#include <msw/type_traits/is_byte_plain.hpp>
#include <msw/type_traits/is_bit_plain.hpp>
#include <msw/zzz/dump/byte_dump.hpp>
#include <msw/zzz/dump/bit_dump.hpp>


namespace msw {
namespace zzz {

    template <typename Range>
    typename std::enable_if<is_byte_plain<typename Range::value_type>::value, std::string>::type
    custom_dump(Range r)
    {
        return byte_dump(r);
    }

    template <typename Range>
    typename std::enable_if<is_bit_plain<typename Range::value_type>::value, std::string>::type
    custom_dump(Range r)
    {
        return bit_dump(r);
    }

}}
