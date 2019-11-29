#pragma once

#include <type_traits>              // std::conditional, std::is_const

#include <msw/zzz/ptr/fwd.hpp>
#include <msw/zzz/bit_ptr/fwd.hpp>


namespace msw {
namespace zzz {

    template <typename T>
    struct bit_ptr_value
    {
        typedef typename std::conditional
        <
              std::is_const<T>::value
            , bit const
            , bit
        >::type type;
    };

    template <typename T, typename U>
    struct bit_ptr_redused
    {
        typedef typename std::conditional
        <
              std::is_const<T>::value
            , U const
            , U
        >::type                       value_type ;
        typedef ptr<value_type>       type       ;
    };
    template <typename T>
    struct bit_ptr_redused<T, bit>
    {
        typedef typename std::conditional
        <
              std::is_const<T>::value
            , bit const
            , bit
        >::type                       value_type ;
        typedef bit_ptr_t
        <
            typename std::conditional
            <
                  std::is_const<T>::value
                , byte const
                , byte
            >::type
        >                             type       ;
    };
    template <typename T>
    struct bit_ptr_redused<T, bit const>
    {
        typedef bit const             value_type ;
        typedef bit_ptr_t<byte const> type       ;
    };

}}
