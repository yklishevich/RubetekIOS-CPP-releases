#pragma once

#include <type_traits>            // std::conditional, std::is_const

#include <msw/plain_traits.hpp>
#include <msw/zzz/block/decl.hpp>
#include <msw/zzz/range/fwd.hpp>


namespace msw {
namespace zzz {

    template <typename T, typename U>
    struct range_redused
    {
        typedef typename std::conditional
        <
            std::is_const<T>::value
            , U const
            , U
        >::type                                              value_type ;
        typedef typename plain_traits<value_type>::pointer   pointer    ;
        typedef typename plain_traits<value_type>::reference reference  ;
        typedef typename plain_traits<value_type>::size_type size_type  ;
        typedef msw::position<value_type>                    position   ;
        typedef range<value_type>                            type       ;
    };

}}
