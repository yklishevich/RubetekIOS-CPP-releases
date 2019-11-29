#pragma once

#include <type_traits>              // std::conditional, std::is_const

#include <msw/zzz/ptr/fwd.hpp>
#include <msw/zzz/bit_ptr/fwd.hpp>


namespace msw {
namespace zzz {

    template <typename T, typename U>
    struct ptr_redused
    {
        typedef typename std::conditional
        <
              std::is_const<T>::value
            , U const
            , U
        >::type                               value_type     ;
        typedef ptr<value_type>               type           ;
        typedef typename type::native_pointer native_pointer ;
    };
    template <typename T>
    struct ptr_redused<T, bit>
    {
    private:
        typedef typename std::conditional
        <
              std::is_const<T>::value
            , byte const
            , byte
        >::type                              native_value_type ;
    public:
        typedef typename std::conditional
        <
              std::is_const<T>::value
            , bit const
            , bit
        >::type                              value_type     ;
        typedef bit_ptr_t<native_value_type> type           ;
        typedef native_value_type*           native_pointer ;
    };
    template <typename T>
    struct ptr_redused<T, bit const>
    {
        typedef bit const                    value_type     ;
        typedef bit_ptr_t<byte const>        type           ;
        typedef byte const*                  native_pointer ;
    };

    template <typename T, typename Ptr>
    struct ptr_common
    {
        typedef typename ptr_redused<T, typename Ptr::value_type>::value_type value_type ;
        typedef typename ptr_redused<T, typename Ptr::value_type>::type       type       ;
    };

}}
