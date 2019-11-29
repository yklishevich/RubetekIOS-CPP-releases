#pragma once

#include <msw/size.hpp>
#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/multimeter.hpp>


namespace msw
{
    struct packet_multimeter
        : noncopyable
    {
        struct snapshot
        {
            msw::multimeter::snapshot bytes   ;
            msw::multimeter::snapshot packets ;
        };

        void add_packet(range<msw::byte const> packet)
        {
            add_packet(packet.size());
        }
        void add_packet(multimeter::distance_type size)
        {
            bytes_.add_distance(size);
            packets_.inc_distance();
        }
        template <typename T>
        void add_packet(size<T> sz)
        {
            add_packet(sz.count());
        }
        void add_packets(multimeter::distance_type size, multimeter::distance_type count)
        {
            bytes_.add_distance(size);
            packets_.add_distance(count);
        }

        multimeter::distance_type bytes_count() const
        {
            return bytes_.distance();
        }
        multimeter::distance_type packets_count() const
        {
            return packets_.distance();
        }

        snapshot make_snapshot()
        {
            return snapshot({bytes_.make_snapshot(), packets_.make_snapshot()});
        }

        bool empty() const
        {
            return packets_.empty();
        }

        void reset()
        {
            bytes_.reset();
            packets_.reset();
        }

    private:

        multimeter bytes_   ;
        multimeter packets_ ;
    };
}
