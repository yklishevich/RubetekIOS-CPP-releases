#pragma once 

#include <type_traits>         // std::is_pointer, std::remove_pointer

#include <msw/zzz/ptr/def.hpp>


namespace msw 
{
    template <typename NativePtr>
    ptr<typename std::remove_pointer<NativePtr>::type> make_ptr(NativePtr p)
    {
        static_assert(std::is_pointer<NativePtr>::value, "NativePtr should be pointer");
        return ptr<typename std::remove_pointer<NativePtr>::type>(p);
    }
    inline ptr<byte> make_ptr(void* p)
    {
        return ptr<byte>(reinterpret_cast<byte*>(p));
    }
    inline ptr<byte const> make_ptr(void const* p)
    {
        return ptr<byte const>(reinterpret_cast<byte const*>(p));
    }
}
