#pragma once

#include <dawn/queue_blocks.hpp>


namespace dawn
{
    struct limited_queue_blocks
        : queue_blocks
    {
        explicit             limited_queue_blocks (msw::size<msw::byte> limit)       ;

        msw::size<msw::byte> capacity             ()                           const ;
        msw::size<msw::byte> free_size            ()                           const ;

        bool                 push                 (const_block)                      ;

    private:
        msw::size<msw::byte> size_limit_ ;
    };
}


namespace dawn
{
    inline limited_queue_blocks::limited_queue_blocks(msw::size<msw::byte> size_limit)
        : size_limit_(size_limit)
    {}

    inline msw::size<msw::byte> limited_queue_blocks::capacity() const
    {
        return size_limit_;
    }
    inline msw::size<msw::byte> limited_queue_blocks::free_size() const
    {
        return capacity() - size();
    }

    inline bool limited_queue_blocks::push(const_block blk)
    {
        if (free_size() < blk.size()) return false;
        queue_blocks::push(blk);
        return true;
    }
}
