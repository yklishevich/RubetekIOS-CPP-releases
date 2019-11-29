#pragma once

#include <set>


namespace dawn
{
    template <typename T, template<typename X, typename = std::allocator<X>> class C>
    std::set<T> to_set(C<T> const& c)
    {
        std::set<T> dst;
        for (auto const& v : c)
            dst.emplace(v);
        return dst;
    }
    //to_list
    //to_vector
}
