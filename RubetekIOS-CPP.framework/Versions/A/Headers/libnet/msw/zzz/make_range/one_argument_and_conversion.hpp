#pragma once

#include <msw/config.hpp>


namespace msw
{
    template <typename T, typename X>
    auto make_range(X&& x) -> decltype(make_range(
#ifdef MSW_NO_TEMPLATE_ID_EXPR
            std::move(x)
#else
            std::forward<X>(x)
#endif
        ).template all<T>())
    {
        return make_range(std::forward<X>(x)).template all<T>();
    }
}
