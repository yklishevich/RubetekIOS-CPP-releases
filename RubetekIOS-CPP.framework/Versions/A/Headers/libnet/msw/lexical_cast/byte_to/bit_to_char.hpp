#pragma once 


namespace msw
{
    inline char bit_to_char(bool v, char zero = '-', char one = '*')
    {
        return v ? one : zero;
    }
}
