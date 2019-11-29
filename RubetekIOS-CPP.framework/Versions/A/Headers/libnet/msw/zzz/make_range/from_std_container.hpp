#pragma once

#include <msw/make_range/from_pointers.hpp>
#include <msw/zzz/make_range/result.hpp>


namespace msw {
namespace zzz {

    template <typename Container>
    typename make_range_result<Container>::type
    make_range_from_std_container(Container&& v)
    {
        return make_range(make_ptr(v.data()), size<typename make_range_result<Container>::value_type_naked>(v.size()));
    }

}}
