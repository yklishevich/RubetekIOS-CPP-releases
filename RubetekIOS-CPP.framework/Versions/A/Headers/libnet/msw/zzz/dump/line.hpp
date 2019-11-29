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
    line_dump_size(Range r)
    {
        return r.has_items() ? (int_to_hex_size<typename Range::value_type>() + 1) * r.size().count() - 1 : 0;
    }

    template <typename Range>
    typename std::enable_if<is_bit_plain<typename Range::value_type>::value, size<char>>::type
    line_dump_size(Range r)
    {
        return (r.size() + r.size().template fit_in<byte>().count() - 1).count();
    }

#ifdef MSW_NO_LAMBDA
    template <typename T>
    struct line_dump_lambda
    {
        typedef T value_type;

        line_dump_lambda(range<char>& str, char splitter, size<char> item_size)
            : str_(str)
            , splitter_(splitter)
            , item_size_(item_size)
        {}
        void operator () (value_type value)
        {
            int_to_fix_hex(value, str_.pop_front(item_size_));
            str_.pop_front() = splitter_;
        }
    private:
        range<char>& str_       ;
        char         splitter_  ;
        size<char>   item_size_ ;
    };
#endif

    template <typename Range>
    typename std::enable_if<is_byte_plain<typename Range::value_type>::value>::type
    line_dump(Range r, range<char> str, char splitter)
    {
        typedef typename Range::value_type value_type;
        if (r.has_items())
        {
            size<char> const item_size = int_to_hex_size<value_type>();

            for_each
            (
                r.from_back_to_front(1)
#ifdef MSW_NO_LAMBDA
                , line_dump_lambda<value_type>(str, splitter, item_size)
#else
                , [&](value_type value)
                {
                    int_to_fix_hex(value, str.pop_front(item_size));
                    str.pop_front() = splitter;
                }
#endif
            );

            int_to_fix_hex(r.back(), str.front(item_size));
        }
    }

    template <typename Range>
    typename std::enable_if<is_bit_plain<typename Range::value_type>::value>::type
    line_dump(Range r, range<char> str, char splitter)
    {
#ifdef MSW_NO_LAMBDA
        struct lambda
        {
            lambda(range<char>& str, char splitter)
                : str_(str)
                , splitter_(splitter)
                , n_(0)
            {}
            void operator () (typename Range::const_reference v)
            {
                str_.pop_front() = bit_to_char(static_cast<bool>(v));
                if (((++n_) %= 8) == 0) str_.pop_front() = splitter_;
            }
        private:
            range<char>& str_      ;
            char         splitter_ ;
            int          n_        ;
        };
#else
        int n = 0;
#endif
        for_each
        (
            r
#ifdef MSW_NO_LAMBDA
            , lambda(str, splitter)
#else
            , [&](typename Range::const_reference v)
            {
                str.pop_front() = bit_to_char(static_cast<bool>(v));
                if (((++n) %= 8) == 0) str.pop_front() = splitter;
            }
#endif
        );
    }

}}
