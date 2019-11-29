#pragma once

#include <string>
#include <type_traits>                       // std::enable_if

#include <msw/size.hpp>
#include <msw/range.hpp>
#include <msw/type_traits/is_byte_plain.hpp>
#include <msw/type_traits/is_bit_plain.hpp>
#include <msw/lexical_cast/ptr_to_str.hpp>


namespace msw {
namespace zzz {

    template <typename Range>
    typename std::enable_if<is_byte_plain<typename Range::value_type>::value, std::string>::type
    mem_info_dimensions(Range r)
    {
        return (sizeof(typename Range::value_type) == 1 ? "" : std::to_string(r.size().count())
            + " x " + std::to_string(size<byte>(sizeof(typename Range::value_type)).count())
            + " B = ")
            + std::to_string(size<byte>(r.size()).count()) + " B";
    }
    template <typename Range>
    typename std::enable_if<is_bit_plain<typename Range::value_type>::value, std::string>::type
    mem_info_dimensions(Range r)
    {
        size<bit> const remind = r.size() % size<byte>(1);
        return std::to_string(r.size().template fit_in_self<byte>().count()) + " B"
            + (remind.null() ? "" : " + " + std::to_string(remind.count()) + " b")
            + " = " + std::to_string(r.size().count()) + " b";
    }

    template <typename Range>
    size<char> mem_info_size(Range r)
    {
        return ptr_to_str_size<typename Range::pointer>() * 2 + 4 + size<char>(mem_info_dimensions(r).size());
    }

    template <typename Range>
    position<char> mem_info(Range r, range<char> str)
    {
        size<char> const item_size = ptr_to_str_size<typename Range::pointer>();
        ptr_to_str(r.begin().to_pointer(), str.pop_front(item_size));
        str.pop_front() = '.';
        str.pop_front() = '.';
        ptr_to_str(r.end().to_pointer(), str.pop_front(item_size));
        str.pop_front() = ':';
        str.pop_front() = ' ';
        copy(mem_info_dimensions(r), str.pop_front(mem_info_dimensions(r).size()));
        return str.begin();
    }

}}
