#pragma once 

#include <msw/size.hpp>


namespace msw 
{
    template <typename T>
    size<char> int_to_hex_size()
    {
        return size<char>(sizeof(T) * 2);
    }
}
