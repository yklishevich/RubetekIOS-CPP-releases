#pragma once

#include <cassert>
#include <cstring>
#include <type_traits>

#include <rubetek/utility/package/package.hpp>


namespace rubetek
{
    template <typename T>
    struct package
    {
        static_assert(std::is_pod<T>::value, "T must be POD");

        static_assert((sizeof(bool) == 1) && (sizeof(int) == 4), "bad pod size");

        static void pack(T const& v, buffer& buf)
        {
            buffer::const_pointer p = reinterpret_cast<buffer::const_pointer>(&v);
            buf.insert(buf.end(), p, p + sizeof(T));
        }
        static bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
        {
            return (end - position) >= static_cast<std::ptrdiff_t>(sizeof(T));
        }
        static buffer::const_iterator unpack(T& v, buffer::const_iterator position, buffer::const_iterator end)
        {
            assert(can_unpack(position, end));
            return unpack_impl(v, position, end);
        }
    private:
        static buffer::const_iterator unpack_impl(T& v, buffer::const_iterator position, buffer::const_iterator)
        {
            //v = *reinterpret_cast<T const*>(&(*position)); // ARM NOT WORK
            std::memcpy(reinterpret_cast<unsigned char*>(&v), &(*position), sizeof(T));
            return position + sizeof(T);
        }
    };
}
