#pragma once 

#include <msw/int.hpp>
#include <msw/ptr.hpp>
#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/bit_ptr.hpp>
#include <msw/lexical_cast/ptr_to_str_size.hpp>
#include <msw/lexical_cast/int_to/fix_hex.hpp>
#include <msw/lexical_cast/byte_to/nibble_to_char.hpp>


namespace msw
{
    template <typename T>
    void ptr_to_str(ptr<T> p, range<char> r)
    {
        int_to_fix_hex(p.to_number(), r);
    }
    template <typename T>
    void ptr_to_str(bit_ptr_t<T> p, range<char> r)
    {
        int_to_fix_hex(reinterpret_cast<uint>(p.base()), r.pop_front(int_to_hex_size<uint>()));
        r.pop_front() = '.';
        r.front() = nibble_to_char(byte(p.offset()));
    }

    template <typename Ptr>
    buffer<char> ptr_to_str(Ptr p)
    {
        buffer<char> buf(ptr_to_str_size<Ptr>() + 1);
        ptr_to_str(p, buf.all());
        buf.back() = 0;
        return buf;
    }
}
