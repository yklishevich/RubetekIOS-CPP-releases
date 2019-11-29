#pragma once

#include <msw/make_range/from_pointers.hpp>


namespace msw
{
    template <typename T, typename Size>
    auto make_range_native(T* p, Size s) -> decltype(make_range(make_ptr(p), msw::size<typename std::remove_const<T>::type>(s)))
    {
        return make_range(make_ptr(p), msw::size<typename std::remove_const<T>::type>(s));
    }
}
