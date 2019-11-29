#pragma once

#include <cstring>
#include <type_traits>

#include <msw/type_traits/is_bit_plain.hpp>
#include <msw/type_traits/is_byte_plain.hpp>


namespace msw {
namespace zzz {
namespace alg {

    template <typename Ptr1, typename Ptr2, typename BytesCount>
    bool native_byte_equal(Ptr1 ptr1, Ptr2 ptr2, BytesCount bytes_count)
    {
        return std::memcmp(ptr1, ptr2, bytes_count) == 0;
    }

    template <typename Ptr1, typename Ptr2, typename BytesCount>
    bool native_bit_equal(Ptr1 ptr1, Ptr2 ptr2, BytesCount bytes_count)
    {
        MSW_ASSERT(0);
        return false;
    }

    template <typename R1, typename R2>
    typename std::enable_if<is_byte_plain<typename R1::value_type>::value, bool>::type
    equal(R1 r1, R2 r2)
    {
        return (r1.size() == r2.size()) && native_byte_equal(r1.begin().to_pointer().native(), r2.begin().to_pointer().native(), size<byte>(r1.size()).count());
    }

    template <typename R1, typename R2>
    typename std::enable_if<is_bit_plain<typename R1::value_type>::value, bool>::type
    equal(R1 r1, R2 r2)
    {
        return (r1.size() == r2.size()) && native_bit_equal(r1.begin().to_pointer(), r2.begin().to_pointer(), r1.size().count());
    }

    template <typename R1, typename R2>
    typename std::enable_if<is_byte_plain<typename R1::value_type>::value, bool>::type
    partial_equal(R1 r1, R2 r2)
    {
        return native_byte_equal(r1.begin().to_pointer().native(), r2.begin().to_pointer().native(), size<byte>(r1.size().min(r2.size())).count());
    }

    template <typename R1, typename R2>
    typename std::enable_if<is_bit_plain<typename R1::value_type>::value, bool>::type
    partial_equal(R1 r1, R2 r2)
    {
        return native_bit_equal(r1.begin().to_pointer(), r2.begin().to_pointer(), r1.size().min(r2.size()).count());
    }

}}}
