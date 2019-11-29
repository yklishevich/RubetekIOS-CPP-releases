#pragma once

#include <type_traits>                  // std::enable_if

#include <msw/assert.hpp>
#include <msw/plain_types.hpp>          // bit
#include <msw/type_traits/is_size.hpp>
#include <msw/zzz/size/decl.hpp>
#include <msw/zzz/size/cast_policy.hpp>


namespace msw        {
namespace zzz        {
namespace count_cast {

    template <typename Src, typename Dest, typename Policy = typename size_cast::policy<Src, Dest>::type>
    struct impl
    {
        static typename size<Dest>::count_type cast(typename size<Src>::count_type count)
        {
            auto const bytes_count = sizeof(Src) * count;
            MSW_ASSERT(bytes_count % sizeof(Dest) == 0);
            return bytes_count / sizeof(Dest);
        }
        static typename size<Dest>::count_type fit_in(typename size<Src>::count_type count)
        {
            auto const bytes_count = sizeof(Src) * count;
            return bytes_count / sizeof(Dest) + (bytes_count % sizeof(Dest) != 0);
        }
        static typename size<Dest>::count_type fit_in_self(typename size<Src>::count_type count)
        {
            auto const bytes_count = sizeof(Src) * count;
            return bytes_count / sizeof(Dest);
        }
    };

    template <typename Src, typename Dest>
    struct impl<Src, Dest, size_cast::policy_enum::inc_base_and_even>
    {
        static typename size<Dest>::count_type cast(typename size<Src>::count_type count)
        {
            static auto const ratio = sizeof(Dest) / sizeof(Src);
            MSW_ASSERT(count % ratio == 0);
            return count / ratio;
        }
        static typename size<Dest>::count_type fit_in(typename size<Src>::count_type count)
        {
            static auto const ratio = sizeof(Dest) / sizeof(Src);
            return count / ratio + (count % ratio != 0);
        }
        static typename size<Dest>::count_type fit_in_self(typename size<Src>::count_type count)
        {
            static auto const ratio = sizeof(Dest) / sizeof(Src);
            return count / ratio;
        }
    };

    template <typename Src, typename Dest>
    struct impl<Src, Dest, size_cast::policy_enum::dec_base_and_even>
    {
        static typename size<Dest>::count_type cast(typename size<Src>::count_type count)
        {
            static auto const ratio = sizeof(Src) / sizeof(Dest);
            return count * ratio;
        }
        static typename size<Dest>::count_type fit_in(typename size<Src>::count_type count)
        {
            return cast(count);
        }
        static typename size<Dest>::count_type fit_in_self(typename size<Src>::count_type count)
        {
            return cast(count);
        }
    };

    template <typename Src, typename Dest>
    struct impl<Src, Dest, size_cast::policy_enum::same>
    {
        static typename size<Dest>::count_type cast(typename size<Src>::count_type count)
        {
            return count;
        }
        static typename size<Dest>::count_type fit_in(typename size<Src>::count_type count)
        {
            return count;
        }
        static typename size<Dest>::count_type fit_in_self(typename size<Src>::count_type count)
        {
            return count;
        }
    };

    template <typename T>
    struct impl<T, T, size_cast::policy_enum::same>
    {
        static typename size<T>::count_type cast(typename size<T>::count_type count)
        {
            return count;
        }
        static typename size<T>::count_type fit_in(typename size<T>::count_type count)
        {
            return count;
        }
        static typename size<T>::count_type fit_in_self(typename size<T>::count_type count)
        {
            return count;
        }
    };

    template <typename Src>
    struct impl<Src, bit, size_cast::policy_enum::byte_to_bit>
    {
        static typename size<bit>::count_type cast(typename size<Src>::count_type count)
        {
            return count * sizeof(Src) * 8;
        }
        static typename size<bit>::count_type fit_in(typename size<Src>::count_type count)
        {
            return cast(count);
        }
        static typename size<bit>::count_type fit_in_self(typename size<Src>::count_type count)
        {
            return cast(count);
        }
    };
    template <typename Dest>
    struct impl<bit, Dest, size_cast::policy_enum::bit_to_byte>
    {
        static typename size<Dest>::count_type cast(typename size<bit>::count_type count)
        {
            MSW_ASSERT(count % (sizeof(Dest) * 8) == 0);
            return count / (sizeof(Dest) * 8);
        }
        static typename size<Dest>::count_type fit_in(typename size<bit>::count_type count)
        {
            auto const bytes_count = count / 8 + (count % 8 != 0);
            return bytes_count / sizeof(Dest) + (bytes_count % sizeof(Dest) != 0);
        }
        static typename size<Dest>::count_type fit_in_self(typename size<bit>::count_type count)
        {
            auto const bytes_count = count / 8;
            return bytes_count / sizeof(Dest);
        }
    };

    template <typename DestBase, typename SrcSize>
    typename std::enable_if<is_size<SrcSize>::value, typename size<DestBase>::count_type>::type
    calc_count(SrcSize size)
    {
        return impl<typename SrcSize::base_type, DestBase>::cast(size.count());
    }
    template <typename DestBase, typename SrcCount>
    typename std::enable_if<!is_size<SrcCount>::value, typename size<DestBase>::count_type>::type
    calc_count(SrcCount count)
    {
        return count;
    }

    template <typename SrcBase, typename DestBase>
    typename size<DestBase>::count_type fit_in(typename size<SrcBase>::count_type count)
    {
        return impl<SrcBase, DestBase>::fit_in(count);
    }

    template <typename SrcBase, typename DestBase>
    typename size<DestBase>::count_type fit_in_self(typename size<SrcBase>::count_type count)
    {
        return impl<SrcBase, DestBase>::fit_in_self(count);
    }

}}}
