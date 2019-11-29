#pragma once

#include <type_traits>                              // std::enable_if

#include <msw/size.hpp>
#include <msw/range.hpp>
#include <msw/config.hpp>
#include <msw/plain_types.hpp>                      // byte
#include <msw/type_traits/is_byte_plain.hpp>
#include <msw/type_traits/is_bit_plain.hpp>
#include <msw/algorithm/for_each.hpp>
#include <msw/lexical_cast/int_to/hex_size.hpp>
#include <msw/lexical_cast/int_to/fix_hex.hpp>
#include <msw/lexical_cast/byte_to/bit_to_char.hpp>


namespace msw {
namespace zzz {

    template <typename Range>
    typename std::enable_if<is_byte_plain<typename Range::value_type>::value, size<char>>::type
    dense_dump_size(Range r)
    {
        return r.has_items() ? int_to_hex_size<typename Range::value_type>() * r.size().count() : 0;
    }

    template <typename Range>
    typename std::enable_if<is_bit_plain<typename Range::value_type>::value, size<char>>::type
    dense_dump_size(Range r)
    {
        return r.size().count();
    }

    template <typename Range>
    typename std::enable_if<is_byte_plain<typename Range::value_type>::value>::type
    dense_dump(Range r, range<char> str)
    {
        typedef typename Range::value_type value_type;
#ifdef MSW_NO_LAMBDA
        struct lambda
        {
            lambda(range<char>& str)
                : str_(str)
            {}
            void operator () (value_type value)
            {
                int_to_fix_hex(value, str_.pop_front(int_to_hex_size<value_type>()));
            }
        private:
            range<char>& str_;
        };
#endif
        for_each
        (
            r
#ifdef MSW_NO_LAMBDA
            , lambda(str)
#else
            , [&](value_type value)
            {
                int_to_fix_hex(value, str.pop_front(int_to_hex_size<value_type>()));
            }
#endif
        );
    }

    template <typename Range>
    typename std::enable_if<is_bit_plain<typename Range::value_type>::value>::type
    dense_dump(Range r, range<char> str)
    {
#ifdef MSW_NO_LAMBDA
        struct lambda
        {
            lambda(range<char>& str)
                : str_(str)
            {}
            void operator () (typename Range::const_reference v)
            {
                str_.pop_front() = bit_to_char(static_cast<bool>(v));
            }
        private:
            range<char>& str_;
        };
#endif
        for_each
        (
            r
#ifdef MSW_NO_LAMBDA
            , lambda(str)
#else
            , [&](typename Range::const_reference v)
            {
                str.pop_front() = bit_to_char(static_cast<bool>(v));
            }
#endif
        );
    }

}}
