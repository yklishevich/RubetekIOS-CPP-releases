#pragma once


namespace msw
{
    template<typename T>
    T& single()
    {
        static T t;
        return t;
    }
}
