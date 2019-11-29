#pragma once

#include <queue>

#include <msw/buffer.hpp>


namespace dawn
{
    struct queue_blocks
    {
        typedef msw::range<msw::byte>       block       ;
        typedef msw::range<msw::byte const> const_block ;

                               queue_blocks ()                    ;
                               queue_blocks (queue_blocks&&)      ;

        bool                   empty        ()              const ;
        bool                   has_blocks   ()              const ;
        std::size_t            count        ()              const ;
        msw::size<msw::byte>   size         ()              const ;

        block                  front        ()                    ;
        const_block            front        ()              const ;
        block                  back         ()                    ;
        const_block            back         ()              const ;

        void                   push         (const_block)         ;
        void                   pop          ()                    ;
        msw::buffer<msw::byte> pull         ()                    ;

    private:
        std::queue<msw::buffer<msw::byte>> blocks_ ;
        msw::size<msw::byte>               size_   ;
    };
}


namespace dawn
{
    inline queue_blocks::queue_blocks()
    {}
    inline queue_blocks::queue_blocks(queue_blocks&& other)
        : blocks_ (std::move(other.blocks_))
        , size_   (std::move(other.size_  ))
    {}

    inline bool queue_blocks::empty() const
    {
        return blocks_.empty();
    }
    inline bool queue_blocks::has_blocks() const
    {
        return !empty();
    }
    inline std::size_t queue_blocks::count() const
    {
        return blocks_.size();
    }
    inline msw::size<msw::byte> queue_blocks::size() const
    {
        return size_;
    }

    inline queue_blocks::block queue_blocks::front()
    {
        return blocks_.front().all();
    }
    inline queue_blocks::const_block queue_blocks::front() const
    {
        return blocks_.front().all();
    }
    inline queue_blocks::block queue_blocks::back()
    {
        return blocks_.back().all();
    }
    inline queue_blocks::const_block queue_blocks::back() const
    {
        return blocks_.back().all();
    }

    inline void queue_blocks::push(const_block blk)
    {
        blocks_.emplace(blk);
        size_ += blk.size();
    }
    inline void queue_blocks::pop()
    {
        size_ -= front().size();
        blocks_.pop();
    }
    inline msw::buffer<msw::byte> queue_blocks::pull()
    {
        size_ -= front().size();
        msw::buffer<msw::byte> blk = std::move(blocks_.front());
        blocks_.pop();
        return std::move(blk);
    }
}
