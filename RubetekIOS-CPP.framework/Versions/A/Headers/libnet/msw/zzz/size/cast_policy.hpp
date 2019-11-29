#pragma once

#include <type_traits>         // std::conditional

#include <msw/plain_types.hpp> // bit


namespace msw       {
namespace zzz       {
namespace size_cast {

    struct policy_enum
    {
        struct general           {} ;
        struct same              {} ;
        struct inc_base_and_even {} ;
        struct dec_base_and_even {} ;
        struct bit_to_byte       {} ;
        struct byte_to_bit       {} ;
        struct bit_to_bit        {} ;
    };

    template <typename Src, typename Dest>
    struct policy
    {
    private:
        enum
        {
              dest_size = sizeof( Dest )
            , src_size  = sizeof( Src  )
        };
    public:
        typedef typename std::conditional
        <
              src_size == dest_size
            , policy_enum::same
            , typename std::conditional
            <
                  (src_size < dest_size) && !(dest_size % src_size)
                , policy_enum::inc_base_and_even
                , typename std::conditional
                <
                      (dest_size < src_size) && !(src_size % dest_size)
                    , policy_enum::dec_base_and_even
                    , policy_enum::general
                >::type
            >::type
        >::type type;
    };

    template <typename Dest>
    struct policy<bit, Dest>
    {
        typedef policy_enum::bit_to_byte type;
    };
    template <typename Src>
    struct policy<Src, bit>
    {
        typedef policy_enum::byte_to_bit type;
    };
    template <>
    struct policy<bit, bit>
    {
        typedef policy_enum::bit_to_bit type;
    };

}}}
