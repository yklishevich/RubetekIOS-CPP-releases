#pragma once 

#include <limits>
#include <functional>
#include <type_traits>

#include <msw/noncopyable.hpp>
#include <msw/proto/spp/packer.hpp>
#include <msw/proto/total_size_of_blocks.hpp>


namespace msw {
namespace spp {

    template <typename SizeType = wbyte>
    struct accum_packer
        : noncopyable
    {
        typedef          packer<SizeType>         spp_packer   ;
        typedef typename spp_packer::size_type    size_type    ;
        typedef typename spp_packer::packet_ready packet_ready ;

        explicit accum_packer(packet_ready packet_ready, size<byte> limit_size = msw::KB * 32, size<byte> offset = 0)
            : spp_packer_(packet_ready, limit_size, offset)
        {
            if (limit_size.count() > std::numeric_limits<size_type>::max()) throw std::runtime_error("large limit size");
        }

        template <typename ...T>
        void add_packet(T&&... v)
        {
            if ((total_size_of_blocks(std::forward<T>(v)...) + sizeof(size_type) + (spp_packer_.empty() ? spp_packer_.offset() : spp_packer_.packet_size())) > spp_packer_.capacity())
            {
                if (spp_packer_.empty()) throw std::runtime_error("large packet");
                spp_packer_.flush();
                add_packet(std::forward<T>(v)...);
            }
            else spp_packer_.put_packet_silent(std::forward<T>(v)...);
        }

        void flush()
        {
            spp_packer_.flush();
        }

    private:
        spp_packer spp_packer_;
    };

    typedef accum_packer<byte>  accum_packer_8  ;
    typedef accum_packer<wbyte> accum_packer_16 ;

}}
