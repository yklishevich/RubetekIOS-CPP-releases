#pragma once 

#include <string>

#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/lexical_cast/int_from/hex.hpp>


namespace msw
{
    template <typename T>
    size<T> hex_cast_size(range<char const> in)
    {
        auto const elem_size = sizeof(T) * 2;
        auto const nibble_count = in.size().count();
        if (nibble_count % elem_size) throw std::runtime_error("string of nibbles must be even count of nibbles in T");
        return nibble_count / elem_size;
    }

    template <typename T>
    position<T> hex_cast(range<char const> in, range<T> out)
    {
        if (out.size() < hex_cast_size<T>(in)) throw std::runtime_error("not enough memory for out range");
        while (in.has_items())
            out.pop_front() = int_from_hex<T>(in.pop_front(sizeof(T) * 2));
        return out.begin();
    }

    template <typename T>
    position<T> hex_cast(std::string const& in, range<T> out)
    {
        return hex_cast<T>(make_range(in), out);
    }

    template <typename T>
    buffer<T> hex_cast(range<char const> in)
    {
        buffer<T> buf(hex_cast_size<T>(in));
        hex_cast(in, buf.all());
        return std::move(buf);
    }

    template <typename T>
    buffer<T> hex_cast(std::string const& in)
    {
        return hex_cast<T>(make_range(in));
    }
}
