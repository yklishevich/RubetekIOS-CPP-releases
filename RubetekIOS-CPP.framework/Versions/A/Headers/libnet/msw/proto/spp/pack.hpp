#pragma once 

#include <limits>
#include <type_traits>

#include <msw/buffer.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw {
namespace spp {

    template <typename SizeType>
    void pack(buffer<byte>& buf, range<byte const> block)
    {
        typedef SizeType size_type;
        static_assert(std::numeric_limits<size_type>::is_integer && std::is_unsigned<size_type>::value, "SizeType must be only unsigned integer type");

        auto const max_block_size = size<byte>(std::numeric_limits<size_type>::max());
        if (block.size() > max_block_size) msw::throw_runtime_error("large block: ", block.size(), " (max permissible size: ", max_block_size, ")");

        buf.push_back(size_type(block.size().count()));
        buf.push_back(block);
    }

    template <typename SizeType, typename T>
    void pack(buffer<byte>& buf, T&& v)
    {
        pack<SizeType>(buf, make_range<byte const>(std::forward<T>(v)));
    }

    template <typename T>
    void pack_8(buffer<byte>& buf, T&& v)
    {
        pack<byte>(buf, std::forward<T>(v));
    }
    template <typename T>
    void pack_16(buffer<byte>& buf, T&& v)
    {
        pack<wbyte>(buf, std::forward<T>(v));
    }
    template <typename T>
    void pack_32(buffer<byte>& buf, T&& v)
    {
        pack<qbyte>(buf, std::forward<T>(v));
    }

    template <typename T>
    buffer<byte> pack_8(T&& v)
    {
        buffer<byte> buf;
        pack_8(buf, std::forward<T>(v));
        return std::move(buf);
    }
    template <typename T>
    buffer<byte> pack_16(T&& v)
    {
        buffer<byte> buf;
        pack_16(buf, std::forward<T>(v));
        return std::move(buf);
    }
    template <typename T>
    buffer<byte> pack_32(T&& v)
    {
        buffer<byte> buf;
        pack_32(buf, std::forward<T>(v));
        return std::move(buf);
    }

}}
