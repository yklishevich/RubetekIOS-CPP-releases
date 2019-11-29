#pragma once

#include <msw/plain_types.hpp>


namespace msw {
namespace zzz {

    template <typename T>
    struct allocated_cluster
    {
        typedef T type;
    };
    template <>
    struct allocated_cluster<bit>
    {
        typedef bit::base_type type;
    };

}}
