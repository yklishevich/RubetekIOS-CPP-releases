#pragma once

#include <functional>

#include <msw/int.hpp>
#include <msw/noncopyable.hpp>
#include <msw/plain_traits.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/dimension/counter.hpp>
#include <msw/proto/spp/unpack.hpp>


namespace msw         {
namespace plain_train {

    template <typename SizeType = wbyte, typename CounterType = wbyte>
    struct unwrapper
        : noncopyable
    {
        typedef SizeType                               size_type    ;
        typedef CounterType                            counter_type ;
        typedef std::function<void(range<byte const>)> block_ready  ;

        explicit unwrapper          (block_ready)             ;

        void     add_packet         (range<byte const>)       ;

        uint     counter_errors     ()                  const ;
        uint     protocol_errors    ()                  const ;

    private:
        void     add_protocol_error ()                        ;

        block_ready           block_ready_     ;
        trigger               first_packet_    ;
        counter<counter_type> counter_         ;
        counter<uint>         counter_errors_  ;
        counter<uint>         protocol_errors_ ;
    };

    typedef unwrapper<byte>  unwrapper_8  ;
    typedef unwrapper<wbyte> unwrapper_16 ;
    typedef unwrapper<qbyte> unwrapper_32 ;

}}


namespace msw         {
namespace plain_train {

    template <typename SizeType, typename CounterType>
    unwrapper<SizeType, CounterType>::unwrapper(block_ready block_ready)
        : block_ready_  ( block_ready )
        , first_packet_ ( true        )
    {}

    template <typename SizeType, typename CounterType>
    void unwrapper<SizeType, CounterType>::add_packet(range<byte const> input_packet)
    {
        if (input_packet.size() < size<counter_type>(1)) add_protocol_error();
        else
        {
            counter_type const field_counter = input_packet.pop_front<counter_type>();
            if (first_packet_) first_packet_.reset();
            else if (field_counter != counter_.value()) ++counter_errors_;
            counter_.set(field_counter + 1);

            spp::unpack_error error;
            spp::unpack<size_type>(input_packet, block_ready_, error);
            if (error != spp::unpack_error::no) add_protocol_error();
        }
    }

    template <typename SizeType, typename CounterType>
    uint unwrapper<SizeType, CounterType>::counter_errors() const
    {
        return counter_errors_.value();
    }
    template <typename SizeType, typename CounterType>
    uint unwrapper<SizeType, CounterType>::protocol_errors() const
    {
        return protocol_errors_.value();
    }

    template <typename SizeType, typename CounterType>
    void unwrapper<SizeType, CounterType>::add_protocol_error()
    {
        ++protocol_errors_;
    }

}}
