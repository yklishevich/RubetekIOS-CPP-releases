#pragma once

#include <type_traits>      // std::remove_reference

#include <msw/bit_ref.hpp>


namespace msw
{
    template <typename T>
    struct remove_reference
        : std::remove_reference<T>
    {};

    template <>
    struct remove_reference<bit_ref>
    {
        typedef bit type;
    };
    template <>
    struct remove_reference<bit_const_ref>
    {
        typedef bit const type;
    };
}
