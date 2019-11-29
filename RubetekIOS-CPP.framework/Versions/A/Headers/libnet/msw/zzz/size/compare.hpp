#pragma once

#include <type_traits>                  // std::enable_if

#include <msw/type_traits/is_size.hpp>
#include <msw/zzz/size/cast_policy.hpp>


namespace msw          {
namespace zzz          {
namespace compare_size {

    template <typename T1, typename T2, typename Policy = typename size_cast::policy<T1, T2>::type>
    struct common
    {
        typedef byte type;
    };

    template <typename T1, typename T2>
    struct common<T1, T2, size_cast::policy_enum::inc_base_and_even>
    {
        typedef T1 type;
    };

    template <typename T1, typename T2>
    struct common<T1, T2, size_cast::policy_enum::dec_base_and_even>
    {
        typedef T2 type;
    };

    template <typename T1, typename T2>
    struct common<T1, T2, size_cast::policy_enum::same>
    {
        typedef T1 type;
    };

    template <typename T, typename Policy>
    struct common<T, T, Policy>
    {
        typedef T type;
    };

}}}
