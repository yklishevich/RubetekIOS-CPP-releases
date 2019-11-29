#pragma once

#include <msw/noncopyable.hpp>
#include <msw/plain_traits.hpp>
#include <msw/dimension/counter.hpp>
#include <msw/proto/spp/accum_packer.hpp>


namespace msw         {
namespace plain_train {

    template <typename SizeType = wbyte, typename CounterType = wbyte>
    struct wrapper
        : noncopyable
    {
        typedef typename spp::accum_packer<SizeType>::packet_ready packet_ready;

                                 explicit wrapper   (packet_ready, size<byte> packet_size) ;
                                 ~        wrapper   ()                                     ;
        template <typename ...T> void     add_block (T&&...)                               ;
                                 void     flush     ()                                     ;
    private:
        counter<CounterType>        counter_      ;
        spp::accum_packer<SizeType> accum_packer_ ;
    };

    typedef wrapper<byte>  wrapper_8  ;
    typedef wrapper<wbyte> wrapper_16 ;
    typedef wrapper<qbyte> wrapper_32 ;
}}


namespace msw         {
namespace plain_train {

    template <typename SizeType, typename CounterType>
    wrapper<SizeType, CounterType>::wrapper(packet_ready pkt_ready, size<byte> packet_size)
        : accum_packer_
        (
            [this, pkt_ready](range<byte> packet)
            {
                packet.front<CounterType>() = (counter_++).value();
                pkt_ready(packet);
            }
            , packet_size
            , sizeof(CounterType)
        )
    {}
    template <typename SizeType, typename CounterType>
    wrapper<SizeType, CounterType>::~wrapper()
    {
        flush();
    }
    template <typename SizeType, typename CounterType>
    template <typename ...T>
    void wrapper<SizeType, CounterType>::add_block(T&&... v)
    {
        accum_packer_.add_packet(std::forward<T>(v)...);
    }
    template <typename SizeType, typename CounterType>
    void wrapper<SizeType, CounterType>::flush()
    {
        accum_packer_.flush();
    }

}}
