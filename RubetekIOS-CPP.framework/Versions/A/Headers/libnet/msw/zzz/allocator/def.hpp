#pragma once

#include <msw/zzz/allocator/decl.hpp>
#include <msw/zzz/block/def.hpp>


namespace msw {
namespace zzz {

    template <typename T>
    typename allocator<T>::cluster_block allocator<T>::allocate(size_type size)
    {
        cluster_size_type const allocated_size = size.template fit_in<cluster_type>();
        cluster_pointer const first(new cluster_type[allocated_size.count()]);
        return cluster_block(first, allocated_size);
    }

}}
