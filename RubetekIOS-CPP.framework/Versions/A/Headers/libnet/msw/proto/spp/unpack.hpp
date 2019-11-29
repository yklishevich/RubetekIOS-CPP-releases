#pragma once 

#include <limits>
#include <type_traits>

#include <msw/range.hpp>
#include <msw/config.hpp>
#include <msw/proto/spp/endian.hpp>


namespace msw {
namespace spp {

    enum class unpack_error
    {
          no
        , bad_header
        , bad_len
    };

    inline void throw_if_error(unpack_error error)
    {
        if (error != unpack_error::no)
        {
                 if (error == unpack_error::bad_header) throw std::runtime_error( "bad SPP packet header" );
            else if (error == unpack_error::bad_len   ) throw std::runtime_error( "bad SPP packet len"    );
            MSW_ASSERT(0);
        }
    }

    template <typename SizeType, typename T>
    range<T> unpack_one(range<T>& packets, unpack_error& error)
    {
        typedef SizeType size_type;
        static_assert(std::numeric_limits<size_type>::is_integer && std::is_unsigned<size_type>::value, "SizeType must be only unsigned integer type");

        error = unpack_error::no;

        static size<byte> const hdr_len_size = size<size_type>(1);

        if (packets.size() < hdr_len_size)
        {
            error = unpack_error::bad_header;
            return range<T>();
        }
        size<byte> const len = packets.template front<size_type>();
        if (packets.size() < (hdr_len_size + len))
        {
            error = unpack_error::bad_len;
            return range<T>();
        }
        packets.template pop_front<size_type>();
        return packets.pop_front(len);
    }

    template <typename SizeType, typename T, typename PacketReady>
    void unpack(range<T> packets, PacketReady packet_ready, unpack_error& error)
    {
        typedef SizeType size_type;
        static_assert(std::numeric_limits<size_type>::is_integer && std::is_unsigned<size_type>::value, "SizeType must be only unsigned integer type");

        error = unpack_error::no;

        while (packets.has_items())
        {
            range<T> pkt = unpack_one<SizeType>(packets, error);
            if (error == unpack_error::no) packet_ready(pkt);
            else break;
        }
    }

    template <typename SizeType, typename T>
    range<T> unpack_one(range<T>& packets)
    {
        unpack_error error;
        range<T> pkt = unpack_one<SizeType>(packets, error);
        throw_if_error(error);
        return pkt;
    }
    template <typename SizeType, typename T, typename PacketReady>
    void unpack(range<T> packets, PacketReady packet_ready)
    {
        unpack_error error;
        unpack<SizeType>(packets, packet_ready, error);
        throw_if_error(error);
    }

    template <typename T>
    range<T> unpack_one_8(range<T>& packets, unpack_error& error)
    {
        return unpack_one<byte>(packets, error);
    }
    template <typename T>
    range<T> unpack_one_8(range<T>& packets)
    {
        return unpack_one<byte>(packets);
    }
    template <typename T>
    range<T> unpack_one_16(range<T>& packets, unpack_error& error)
    {
        return unpack_one<wbyte>(packets, error);
    }
    template <typename T>
    range<T> unpack_one_16(range<T>& packets)
    {
        return unpack_one<wbyte>(packets);
    }
    template <typename T>
    range<T> unpack_one_32(range<T>& packets, unpack_error& error)
    {
        return unpack_one<qbyte>(packets, error);
    }
    template <typename T>
    range<T> unpack_one_32(range<T>& packets)
    {
        return unpack_one<qbyte>(packets);
    }

    template <typename T, typename PacketReady>
    void unpack_8(range<T> packets, PacketReady packet_ready, unpack_error& error)
    {
        unpack<byte>(packets, packet_ready, error);
    }
    template <typename T, typename PacketReady>
    void unpack_8(range<T> packets, PacketReady packet_ready)
    {
        unpack<byte>(packets, packet_ready);
    }
    template <typename T, typename PacketReady>
    void unpack_16(range<T> packets, PacketReady packet_ready, unpack_error& error)
    {
        unpack<wbyte>(packets, packet_ready, error);
    }
    template <typename T, typename PacketReady>
    void unpack_16(range<T> packets, PacketReady packet_ready)
    {
        unpack<wbyte>(packets, packet_ready);
    }
    template <typename T, typename PacketReady>
    void unpack_32(range<T> packets, PacketReady packet_ready, unpack_error& error)
    {
        unpack<qbyte>(packets, packet_ready, error);
    }
    template <typename T, typename PacketReady>
    void unpack_32(range<T> packets, PacketReady packet_ready)
    {
        unpack<qbyte>(packets, packet_ready);
    }

}}
