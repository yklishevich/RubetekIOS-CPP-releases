#pragma once

#include <functional>

#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw          {
namespace large_packet {

    struct unwrapper
        : noncopyable
    {
        typedef std::function<void(range<byte const>)> packet_ready;

        explicit unwrapper  (packet_ready)      ;
        void     add_packet (range<byte const>) ;

    private:

        packet_ready const packet_ready_ ;
        buffer<byte>       packet_       ;
    };

}}


namespace msw          {
namespace large_packet {

    inline unwrapper::unwrapper(packet_ready packet_ready)
        : packet_ready_    (packet_ready)
    {}
    inline void unwrapper::add_packet(range<byte const> packet)
    {
        if (packet.size() < size<byte>(2)) throw_runtime_error("very small packet size: ", packet.size().count());
        bool not_last = packet_.pop_front() != 0;
        if (not_last) packet_.push_back(packet);
        else
        {
            if (packet_.has_items())
            {
                packet_.push_back(packet);
                packet_ready_(packet_.all());
                packet_.clear();
            }
            else packet_ready_(packet);
        }
    }

}}
