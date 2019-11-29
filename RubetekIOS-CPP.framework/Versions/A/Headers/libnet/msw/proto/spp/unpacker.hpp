#pragma once 

#include <msw/buffer.hpp>
#include <msw/proto/spp/unpack.hpp>


namespace msw {
namespace spp {

    template <typename SizeType, typename T = byte>
    struct unpacker
    {
        typedef T                                value_type   ;
        typedef range<value_type const>          const_range  ;
        typedef std::function<void(const_range)> packet_ready ;

        explicit unpacker(packet_ready packet_ready)
            : packet_ready_(packet_ready)
        {}

        void unpack(const_range packets)
        {
            if (buf_.empty())
                zzz_unpack(packets);
            else
            {
                buf_.push_back(packets);
                zzz_unpack(buf_.all());
            }
        }
        void reset()
        {
            buf_.clear();
        }

    private:

        void zzz_unpack(const_range packets)
        {
            while(packets.has_items())
            {
                unpack_error error;
                const_range pkt = unpack_one<SizeType>(packets, error);
                if (error == unpack_error::no)
                    packet_ready_(pkt);
                else
                {
                    buf_ = packets;
                    return;
                }
            }
            buf_.clear();
        }

        buffer<value_type>  buf_            ;
        packet_ready        packet_ready_   ;
    };

    typedef unpacker<byte>  unpacker_8  ;
    typedef unpacker<wbyte> unpacker_16 ;
    typedef unpacker<qbyte> unpacker_32 ;

}}
