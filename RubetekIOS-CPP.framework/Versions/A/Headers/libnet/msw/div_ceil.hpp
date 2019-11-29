#pragma once


namespace msw
{
    template <typename T>
    T div_ceil(T dividend, T divider)
    {
        return dividend / divider + (dividend % divider != 0);
    }
}
