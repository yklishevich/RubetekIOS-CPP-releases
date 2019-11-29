#pragma once 

#include <type_traits>                  // std::enable_if

#include <msw/zzz/block/def.hpp>
#include <msw/type_traits/is_ptr.hpp>
#include <msw/type_traits/is_size.hpp>


namespace msw {
namespace zzz {

    template <typename X1, typename X2>
    struct make_block_result
    {};
    template <typename T1, typename T2>
    struct make_block_result<ptr<T1>, ptr<T2>>
    {
        typedef block<typename zzz::ptr_common<T1, ptr<T2>>::value_type> type;
    };
    template <typename T1, typename T2>
    struct make_block_result<ptr<T1>, size<T2>>
    {
        typedef block<T1> type;
    };
    template <typename T1, typename T2>
    struct make_block_result<size<T1>, ptr<T2>>
    {
        typedef block<T2> type;
    };
    template <typename T1, typename T2>
    struct make_block_result<bit_ptr_t<T1>, bit_ptr_t<T2>>
    {
        typedef block<typename zzz::ptr_common<T1, bit_ptr_t<T2>>::value_type> type;
    };
    template <>
    struct make_block_result<bit_ptr, bit_size>
    {
        typedef block<bit> type;
    };
    template <>
    struct make_block_result<bit_size, bit_ptr>
    {
        typedef block<bit> type;
    };
    template <>
    struct make_block_result<bit_const_ptr, bit_size>
    {
        typedef block<bit const> type;
    };
    template <>
    struct make_block_result<bit_size, bit_const_ptr>
    {
        typedef block<bit const> type;
    };

    template <typename Ptr1, typename Ptr2>
    typename std::enable_if
    <
        is_ptr<Ptr1>::value && is_ptr<Ptr2>::value
        , typename make_block_result<Ptr1, Ptr2>::type
    >::type
    make_block(Ptr1 first, Ptr2 last)
    {
        typedef typename zzz::ptr_common<typename Ptr1::value_type, Ptr2>::type pointer;
        return typename make_block_result<Ptr1, Ptr2>::type(static_cast<pointer>(first), static_cast<pointer>(last));
    }

    template <typename Ptr, typename Size>
    typename std::enable_if
    <
        is_ptr<Ptr>::value && is_size<Size>::value
        , typename make_block_result<Ptr, Size>::type
    >::type
    make_block(Ptr first, Size sz)
    {
        return typename make_block_result<Ptr, Size>::type(first, sz);
    }

    template <typename Size, typename Ptr>
    typename std::enable_if
    <
        is_size<Size>::value && is_ptr<Ptr>::value
        , typename make_block_result<Size, Ptr>::type
    >::type
    make_block(Size sz, Ptr last)
    {
        return typename make_block_result<Size, Ptr>::type(sz, last);
    }

}}
