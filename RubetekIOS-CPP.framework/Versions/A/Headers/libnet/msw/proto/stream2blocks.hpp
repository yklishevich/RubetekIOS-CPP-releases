#pragma once

#include <functional>

#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>


namespace msw
{
    template <typename ByteType = byte>
    struct stream2multiblocks
        : noncopyable
    {
        typedef ByteType                        byte_type   ;
        typedef range<byte_type>                block_type  ;
        typedef std::function<void(block_type)> block_ready ;

                   stream2multiblocks (size<byte>, block_ready)       ;
        void       process            (block_type)                    ;
        size<byte> block_size         ()                        const ;

    private:
        block_ready  const block_ready_    ;
        buffer<byte>       receive_buffer_ ;
    };

    template <typename ByteType = byte>
    struct stream2blocks
        : stream2multiblocks<ByteType>
    {
        typedef typename stream2multiblocks<ByteType>::byte_type   byte_type   ;
        typedef typename stream2multiblocks<ByteType>::block_type  block_type  ;
        typedef typename stream2multiblocks<ByteType>::block_ready block_ready ;

        stream2blocks (size<byte>, block_ready) ;
    };
}


namespace msw
{
    template <typename ByteType>
    stream2multiblocks<ByteType>::stream2multiblocks(size<byte> block_size, block_ready block_ready)
        : block_ready_    ( block_ready   )
        , receive_buffer_ ( 0, block_size )
    {}
    template <typename ByteType>
    void stream2multiblocks<ByteType>::process(block_type block)
    {
        if (receive_buffer_.has_items())
        {
            receive_buffer_.push_back(block.pop_front_max(receive_buffer_.free_space()));
            if (receive_buffer_.no_free_space())
            {
                block_ready_(receive_buffer_.all());
                receive_buffer_.clear();
                process(block);
            }
        }
        else
        {
            size<byte> const tail_size = block.size() % receive_buffer_.capacity();
            receive_buffer_.push_back_silent(block.pop_back(tail_size));
            if (block.has_items()) block_ready_(block);
        }
    }
    template <typename ByteType>
    size<byte> stream2multiblocks<ByteType>::block_size() const
    {
        return receive_buffer_.capacity();
    }

    template <typename ByteType>
    stream2blocks<ByteType>::stream2blocks(size<byte> block_size, block_ready block_ready)
        : stream2multiblocks<ByteType>
        (
            block_size
            , [block_size, block_ready](range<byte const> block)
            {
                while (block.has_items()) block_ready(block.pop_front(block_size));
            }
        )
    {}
}
