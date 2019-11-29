#pragma once


namespace msw
{
    template <typename T, typename X1, typename X2>
    auto make_range(X1 x1, X2 x2) -> decltype(make_range(x1, x2).template all<T>())
    {
        return make_range(x1, x2).template all<T>();
    }
}
