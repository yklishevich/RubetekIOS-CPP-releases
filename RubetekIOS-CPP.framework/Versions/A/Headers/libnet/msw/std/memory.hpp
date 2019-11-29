#pragma once

#include <memory>
#include <type_traits>

#include <msw/config.hpp>


namespace msw
{
#ifdef MSW_MAKE_UNIQUE
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
#else
    using std::make_unique;
#endif
}

namespace msw
{
    template<typename T, typename U, typename ...Args>
    std::unique_ptr<T> make_unique_inherited(Args&&... args)
    {
        return std::unique_ptr<T>(new U(std::forward<Args>(args)...));
    }
}

#ifdef MSW_GCC
#   define MSW_SHARED_FROM_THIS std::enable_shared_from_this<typename std::remove_reference<decltype(*this)>::type>::shared_from_this()
#else
#   define MSW_SHARED_FROM_THIS shared_from_this()
#endif
