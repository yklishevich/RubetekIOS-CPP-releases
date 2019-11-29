#pragma once 

#include <limits>
#include <type_traits>  // std::is_signed

#include <msw/size.hpp>


namespace msw
{
    template <typename T>
    size<char> int_to_dec_size()
    {
        return std::numeric_limits<T>::digits10 + 1 + (std::is_unsigned<T>::value ? 0 : 1);
    }
}
