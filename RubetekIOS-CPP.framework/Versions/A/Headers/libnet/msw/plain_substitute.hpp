#pragma once 

#include <type_traits>

#include <msw/plain_types.hpp>
#include <msw/type_traits/is_plain.hpp>


namespace msw
{
    template <typename T>
    struct plain_substitute
    {
        static_assert(is_plain<T>::value, "T must be plain type");
        static_assert(sizeof(T) <= sizeof(ebyte), "size of T must be not more size of ebyte");

        typedef typename std::conditional
        <
            sizeof(T) == sizeof(byte)
            , byte
            , typename std::conditional
            <
                sizeof(T) == sizeof(wbyte)
                , wbyte
                , typename std::conditional
                <
                    sizeof(T) == sizeof(qbyte)
                    , qbyte
                    , typename std::conditional
                    <
                        sizeof(T) == sizeof(ebyte)
                        , ebyte
                        , void
                    >::type
                >::type
            >::type
        >::type type;
    };
}
