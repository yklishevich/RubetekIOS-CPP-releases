#pragma once

#include <functional>

#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/scoped_shut.hpp>
#include <msw/dimension/trigger.hpp>


namespace msw
{
    template <typename ByteType = byte>
    struct elastic_receiver
        : noncopyable
    {
        typedef ByteType                        byte_type            ;
        typedef range<byte_type>                block_type           ;
        typedef std::function<void(block_type)> on_receive           ;
        //typedef std::function<void()>         on_receive_completed ;

                   elastic_receiver     ()                                           ;

        void       external_on_receive  (block_type)                                 ;

        void       receive              (on_receive)                                 ;
        void       receive              (size<byte> required_size, on_receive)       ;
        void       receive_still        (on_receive)                                 ;
        void       receive_still        (size<byte> required_size, on_receive)       ;
        void       receive_untill       (range<byte const>, on_receive)              ;
        void       receive_still_untill (range<byte const>, on_receive)              ;

        size<byte> capacity             ()                                     const ;
        void       reserve              (size<byte>)                                 ;

    private:

        struct receive_context
            : noncopyable
        {
                       receive_context ()                                           ;
            void       set             (on_receive)                                 ;
            void       set             (on_receive, size<byte> required_size)       ;
            void       set_still       (on_receive)                                 ;
            void       set_still       (on_receive, size<byte> required_size)       ;
            bool       call            (block_type)                                 ;
            bool       call_performed  ()                                     const ;
            size<byte> required_size   ()                                     const ;
            bool       empty           ()                                     const ;
        private:
            void       clear           ()                                           ;

            on_receive     on_receive_        ;
            size<byte>     required_size_     ;
            //buffer<byte> required_sequence_ ;
            trigger        receive_still_     ;
            trigger        call_performed_    ;
        };

        void process_taken_block    (block_type&) ;
        void process_receive_buffer ()            ;

        receive_context receive_context_ ;
        buffer<byte>    receive_buffer_  ;
    };
}


namespace msw
{
    template <typename ByteType>
    elastic_receiver<ByteType>::elastic_receiver()
    {}
    template <typename ByteType>
    void elastic_receiver<ByteType>::external_on_receive(block_type block)
    {
        if (receive_context_.empty())
        {
            receive_buffer_.push_back(block);
        }
        else
        {
            if (receive_buffer_.empty() && (receive_context_.required_size().null() || (receive_context_.required_size() == block.size())))
            {
                if (!receive_context_.call(block)) receive_buffer_.push_back(block);
            }
            else
            {
                receive_buffer_.push_back(block);
                process_receive_buffer();
            }
        }
    }

    template <typename ByteType>
    void elastic_receiver<ByteType>::receive(on_receive on_receive)
    {
        receive_context_.set(on_receive);
        if (!receive_context_.call_performed()) process_receive_buffer();
    }
    template <typename ByteType>
    void elastic_receiver<ByteType>::receive(size<byte> required_size, on_receive on_receive)
    {
        receive_context_.set(on_receive, required_size);
        if (!receive_context_.call_performed()) process_receive_buffer();
    }
    template <typename ByteType>
    void elastic_receiver<ByteType>::receive_still(on_receive on_receive)
    {
        receive_context_.set_still(on_receive);
        if (!receive_context_.call_performed()) process_receive_buffer();
    }
    template <typename ByteType>
    void elastic_receiver<ByteType>::receive_still(size<byte> required_size, on_receive on_receive)
    {
        receive_context_.set_still(on_receive, required_size);
        if (!receive_context_.call_performed()) process_receive_buffer();
    }

    template <typename ByteType>
    size<byte> elastic_receiver<ByteType>::capacity() const
    {
        return receive_buffer_.capacity();
    }
    template <typename ByteType>
    void elastic_receiver<ByteType>::reserve(size<byte> sz)
    {
        receive_buffer_.reserve(sz);
    }

    template <typename ByteType>
    void elastic_receiver<ByteType>::process_taken_block(block_type& block)
    {
        if (block.has_items() && !receive_context_.empty())
        {
            if (receive_context_.required_size().null())
            {
                if (receive_context_.call(block)) block.shrink_backward();
            }
            else
            {
                size<byte> const required_size = receive_context_.required_size();
                if (block.size() >= required_size)
                {
                    if (receive_context_.call(block.front(required_size))) block.pop_front(required_size);
                    process_taken_block(block);
                }
            }
        }
    }
    template <typename ByteType>
    void elastic_receiver<ByteType>::process_receive_buffer()
    {
        block_type taken_block = receive_buffer_.all();
        process_taken_block(taken_block);
        if (!receive_context_.empty() && (receive_context_.required_size() > capacity()))
        {
            buffer<byte> tmp(0, receive_context_.required_size());
            tmp.push_back(taken_block);
            receive_buffer_.swap(tmp);
        }
        else
        {
            receive_buffer_ = taken_block;
        }
    }
}

namespace msw
{
    template <typename ByteType>
    elastic_receiver<ByteType>::receive_context::receive_context()
        : on_receive_   (nullptr)
        , required_size_(0)
    {}
    template <typename ByteType>
    void elastic_receiver<ByteType>::receive_context::set(on_receive on_receive)
    {
        MSW_ASSERT(on_receive);
        on_receive_ = on_receive;
    }
    template <typename ByteType>
    void elastic_receiver<ByteType>::receive_context::set(on_receive on_receive, size<byte> required_size)
    {
        set(on_receive);
        MSW_ASSERT(required_size.not_null());
        required_size_ = required_size;
    }
    template <typename ByteType>
    void elastic_receiver<ByteType>::receive_context::set_still(on_receive on_receive)
    {
        set(on_receive);
        receive_still_.set();
    }
    template <typename ByteType>
    void elastic_receiver<ByteType>::receive_context::set_still(on_receive on_receive, size<byte> required_size)
    {
        set(on_receive, required_size);
        receive_still_.set();
    }
    template <typename ByteType>
    bool elastic_receiver<ByteType>::receive_context::call(block_type block)
    {
        MSW_ASSERT(!empty());
        MSW_ASSERT(block.has_items());
        if (required_size_.not_null())
        {
            MSW_ASSERT(required_size_ == block.size());
        }
        {
            on_receive current_on_receive(on_receive_);
            clear();
            {
                call_performed_.set();
                scoped_shut scoped_shut
                (
                    [this]()
                    {
                        call_performed_.reset();
                    }
                );
                current_on_receive(block);
            }
        }
        if (receive_still_)
        {
            if (required_size_.not_null()) required_size_ += block.size();
            return false;
        }
        return true;
    }
    template <typename ByteType>
    bool elastic_receiver<ByteType>::receive_context::call_performed() const
    {
        return static_cast<bool>(call_performed_);
    }
    template <typename ByteType>
    size<byte> elastic_receiver<ByteType>::receive_context::required_size() const
    {
        MSW_ASSERT(!empty());
        return required_size_;
    }
    template <typename ByteType>
    bool elastic_receiver<ByteType>::receive_context::empty() const
    {
        return on_receive_ == nullptr;
    }

    template <typename ByteType>
    void elastic_receiver<ByteType>::receive_context::clear()
    {
        on_receive_ = nullptr;
        required_size_.clear();
        receive_still_.reset();
    }
}
