#pragma once 


namespace msw
{
    template <typename T>
    T ipow(T v, int degree)
    {
        if (degree)
        {
            T m = v;
            while (--degree) v *= m;
            return v;
        }
        return 1;
    }
}
