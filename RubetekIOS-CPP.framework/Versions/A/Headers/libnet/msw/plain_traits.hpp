#pragma once 

#include <type_traits>                  // std::remove_const

#include <msw/ptr.hpp>
#include <msw/size.hpp>
#include <msw/bit_ref.hpp>
#include <msw/bit_ptr.hpp>
#include <msw/plain_types.hpp>
#include <msw/type_traits/is_plain.hpp>


namespace msw
{
    template <typename T>
    struct plain_traits
    {
        static_assert(is_plain<T>::value, "T must be plain type");

        typedef T                                         value_type      ;
        typedef ptr<T>                                    pointer         ;
        typedef ptr<T const>                              const_pointer   ;
        typedef T&                                        reference       ;
        typedef T const&                                  const_reference ;
        typedef size<typename std::remove_const<T>::type> size_type       ;
    };

    template <>
    struct plain_traits<bit>
    {
        typedef bit           value_type      ;
        typedef bit_ptr       pointer         ;
        typedef bit_const_ptr const_pointer   ;
        typedef bit_ref       reference       ;
        typedef bit_const_ref const_reference ;
        typedef bit_size      size_type       ;
    };
    template <>
    struct plain_traits<bit const>
    {
        typedef bit const     value_type      ;
        typedef bit_const_ptr pointer         ;
        typedef bit_const_ptr const_pointer   ;
        typedef bit_const_ref reference       ;
        typedef bit_const_ref const_reference ;
        typedef bit_size      size_type       ;
    };
}