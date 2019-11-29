#pragma once

#include <type_traits>                  // std::conditional, std::remove_reference, std::remove_const, std::is_const

#include <msw/type_traits/is_plain.hpp>
#include <msw/zzz/range/decl.hpp>


namespace msw {
namespace zzz {

    template <typename X>
    struct make_range_result
    {
    private:
        template <typename T, bool = is_plain<T>::value>
        struct value
        {
            typedef typename T::value_type type;
        };
        template <typename T>
        struct value<T, true>
        {
            typedef T type;
        };
        typedef typename std::remove_reference<X>::type     x_non_ref ;
        typedef typename std::remove_const<x_non_ref>::type x_naked   ;
    public:
        typedef typename value<x_naked>::type               value_type_naked ;
        typedef typename std::conditional
        <
            std::is_const<x_non_ref>::value
            , value_type_naked const
            , value_type_naked
        >::type                                             value_type ;
        typedef range<value_type>                           type       ;
    };

}}
