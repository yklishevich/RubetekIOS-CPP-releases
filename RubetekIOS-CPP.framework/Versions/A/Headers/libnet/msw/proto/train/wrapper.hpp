#pragma once

#include <string>
#include <functional>

#include <msw/assert.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/plain_traits.hpp>        // byte, wbyte, qbyte
#include <msw/dimension/counter.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/proto/train/headers.hpp>


namespace msw   {
namespace train {

    struct wrapper
        : noncopyable
    {
        typedef  std::function<void(range<byte>)> packet_ready;

        explicit wrapper   (packet_ready)                                       ;
                 wrapper   (size<byte> packet_size, packet_ready)               ;
        ~        wrapper   ()                                                   ;

        void     add_block (range<byte const>)                                  ;
        void     add_block (range<byte const>, qbyte pid, qbyte lid)            ;
        void     add_block (range<byte const>, qbyte pid, qbyte lid, wbyte ext) ;

        void     flush     ()                                                   ;

    private:

        void     add_wagon (range<byte const>)                                  ;

        size<byte>     const packet_size_  ;
        packet_ready         packet_ready_ ;
        buffer<byte>         packet_       ;
        buffer<byte>         wagon_        ;
        wagon_header&        wagon_header_ ;
        counter<wbyte>       counter_      ;

    };

}}


namespace msw   {
namespace train {

    inline wrapper::wrapper(packet_ready packet_ready)
        : wrapper(default_packet_size, packet_ready)
    {}
    inline wrapper::wrapper(size<byte> packet_size, packet_ready packet_ready)
        : packet_size_(packet_size)
        , packet_ready_(packet_ready)
        , packet_(0, packet_size_)
        , wagon_(size<wagon_header>(1), wagon_max_size)
        , wagon_header_(wagon_.front<wagon_header>())
    {
        if (packet_size.not_even(size<byte>(4)) || (packet_size < min_packet_size) || (packet_size > max_packet_size))
            throw_runtime_error("invalid packet size: ", packet_size);
    }
    inline wrapper::~wrapper()
    {
        flush();
    }

    inline void wrapper::add_block(range<byte const> block)
    {
        add_block(block, 0, 0);
    }
    inline void wrapper::add_block(range<byte const> block, qbyte pid, qbyte lid)
    {
        add_block(block, pid, lid, 0);
    }
    inline void wrapper::add_block(range<byte const> block, qbyte pid, qbyte lid, wbyte ext)
    {
        if (block.size() >= (wagon_max_size - size<wagon_header>(1)))
            throw_runtime_error("inadmissible size of block: ", block.size());

        wagon_header_.len = static_cast<wbyte>((block.size() + 8).count());
        wagon_header_.ext = ext;
        wagon_header_.pid = pid;
        wagon_header_.lid = lid;

        wagon_.push_back_silent(block);
        wagon_.increase_size_silent(block.size().mod_back(4));
        add_wagon(wagon_.all());
        wagon_.resize_silent(size<wagon_header>(1));
    }

    inline void wrapper::flush()
    {
        if (packet_.has_items())
        {
            packet_ready_(packet_.all());
            packet_.clear();
        }
    }

    inline void wrapper::add_wagon(range<byte const> wagon)
    {
        MSW_ASSERT((wagon.size() % 4).null());
        range<byte const> remind_part_of_wagon(wagon);

        while (remind_part_of_wagon.has_items())
        {
            if (packet_.empty())
            {
                packet_.increase_size_silent(size<packet_header>(1));
                packet_header& header = packet_.front<packet_header>();
                header.counter = (counter_++).value();
                size<byte> const remind_part_of_wagon_size = remind_part_of_wagon.size();
                header.pointer = wagon.size() == remind_part_of_wagon_size
                    ? 0
                    : ((remind_part_of_wagon_size >= (packet_size_ - 4))
                        ? 0xffff
                        : static_cast<wbyte>(remind_part_of_wagon_size.count()));
            }

            size<byte> const free_space_in_packet = packet_size_ - packet_.size();
            packet_.push_back_silent(remind_part_of_wagon.pop_front_max(free_space_in_packet));

            MSW_ASSERT(packet_size_ >= packet_.size());
            if (packet_size_ == packet_.size()) flush();
        }
    }

}}
