#pragma once


namespace rubetek {
namespace ctti    {
namespace zzz     {

    template <typename T>
    struct global_type_id_impl;

    template <int>
    struct guarantor_uniqueness;

}}}

#define CTTI_GLOBAL_TYPEID(T, ID)\
namespace rubetek {\
namespace ctti {\
namespace zzz {\
    template <>\
    struct global_type_id_impl<T>\
    {\
        static int const id = ID;\
    };\
    template <>\
    struct guarantor_uniqueness<ID>{};\
}}}

namespace rubetek {
namespace ctti {

    template <typename T>
    int global_typeid()
    {
        return zzz::global_type_id_impl<T>::id;
    }

}}
