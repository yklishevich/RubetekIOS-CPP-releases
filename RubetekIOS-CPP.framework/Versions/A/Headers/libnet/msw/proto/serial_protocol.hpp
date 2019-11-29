#pragma once

#include <functional>

#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>


namespace msw
{
    template <typename In = byte, typename Out = In>
    struct serial_protocol
        : noncopyable
    {
        typedef In                             in_value   ;
        typedef Out                            out_value  ;
        typedef range<in_value>                in_range   ;
        typedef range<out_value>               out_range  ;
        typedef std::function<void(out_range)> on_send    ;
        typedef std::function<void()>          on_sent    ;
        typedef std::function<void(in_range)>  on_receive ;

        explicit serial_protocol     (on_send, on_receive = nullptr, on_sent = nullptr) ;

        void     external_on_receive (in_range)                                         ;
        void     external_on_sent    ()                                                 ;

        void     send                (in_range)                                         ;
        void     send                (in_range, on_sent)                                ;
        void     receive             (on_receive on_receive = nullptr)                  ;
        void     receive             (size<byte> required_size, on_receive)             ;
        void     receive_append      (on_receive on_receive)                            ;
        void     receive_append      (size<byte> required_size, on_receive)             ;

        void     reset               ()                                                 ;

    private:
        void     call_on_receive     (in_range)                                         ;
        void     process_taken_block (in_range&)                                        ;

        on_send      on_send_        ;
        on_receive   on_receive_     ;
        on_sent      on_sent_        ;
        size<byte>   required_size_  ;
        buffer<byte> receive_buffer_ ;
    };
}


namespace msw
{
    template <typename In, typename Out>
    serial_protocol<In, Out>::serial_protocol(on_send on_send, on_receive on_receive, on_sent on_sent)
        : on_send_       ( on_send    )
        , on_receive_    ( on_receive )
        , on_sent_       ( on_sent    )
        , required_size_ ( 0          )
    {}
    template <typename In, typename Out>
    void serial_protocol<In, Out>::external_on_receive(in_range block)
    {
        if (required_size_.null() || (required_size_ == block.size())) call_on_receive(block);
        else
        {
            receive_buffer_.push_back(block);
            in_range taken_block = receive_buffer_.all();
            process_taken_block(taken_block);
            if (required_size_.null()) receive_buffer_.clear();
            else
            {
                if (required_size_ > receive_buffer_.capacity())
                {
                    buffer<byte> tmp(0, required_size_);
                    tmp.push_back(taken_block);
                    receive_buffer_.swap(tmp);
                }
                else receive_buffer_ = taken_block;
            }
        }
    }
    template <typename In, typename Out>
    void serial_protocol<In, Out>::external_on_sent()
    {
        if (on_sent_) on_sent_();
    }

    template <typename In, typename Out>
    void serial_protocol<In, Out>::send(in_range block)
    {
        on_send_(block);
    }
    template <typename In, typename Out>
    void serial_protocol<In, Out>::send(in_range block, on_sent on_sent)
    {
        on_sent_ = on_sent;
        on_send_(block);
    }
    template <typename In, typename Out>
    void serial_protocol<In, Out>::receive(on_receive on_receive)
    {
        on_receive_ = on_receive;
        required_size_.clear();
    }
    template <typename In, typename Out>
    void serial_protocol<In, Out>::receive(size<byte> required_size, on_receive on_receive)
    {
        MSW_ASSERT(required_size.not_null());
        MSW_ASSERT(on_receive);
        receive(on_receive);
        required_size_ = required_size;
    }
    //template <typename In, typename Out>
    //void serial_protocol<In, Out>::receive_append(on_receive on_receive)
    //{
    //  
    //}
    //template <typename In, typename Out>
    //void serial_protocol<In, Out>::receive_append(size<byte> required_size, on_receive on_receive)
    //{
    //  
    //}

    template <typename In, typename Out>
    void serial_protocol<In, Out>::reset()
    {
        required_size_ = 0;
        receive_buffer_.clear();
    }

    template <typename In, typename Out>
    void serial_protocol<In, Out>::call_on_receive(in_range block)
    {
        if (on_receive_) (on_receive(on_receive_))(block);
    }
    template <typename In, typename Out>
    void serial_protocol<In, Out>::process_taken_block(in_range& block)
    {
        if (required_size_.null())
        {
            call_on_receive(block);
            block.shrink_backward();
        }
        else if (block.size() >= required_size_)
        {
            call_on_receive(block.pop_front(required_size_));
            process_taken_block(block);
        }
    }
}
