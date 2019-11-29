#pragma once

#include <msw/plain_traits.hpp>
#include <msw/zzz/allocator/cluster.hpp>
#include <msw/zzz/block/decl.hpp>


namespace msw {
namespace zzz {

    template <typename T>
    struct allocator
    {
        typedef typename plain_traits<T>::value_type           value_type        ;
        typedef typename plain_traits<T>::pointer              pointer           ;
        typedef typename plain_traits<T>::const_pointer        const_pointer     ;
        typedef typename plain_traits<T>::reference            reference         ;
        typedef typename plain_traits<T>::const_reference      const_reference   ;
        typedef typename plain_traits<T>::size_type            size_type         ;

        typedef typename allocated_cluster<T>::type            cluster_type      ;
        typedef typename plain_traits<cluster_type>::pointer   cluster_pointer   ;
        typedef typename plain_traits<cluster_type>::size_type cluster_size_type ;
        typedef typename zzz::block<cluster_type>              cluster_block     ;

        static cluster_block allocate(size_type);

    private:
        ~allocator();
    };

}}
