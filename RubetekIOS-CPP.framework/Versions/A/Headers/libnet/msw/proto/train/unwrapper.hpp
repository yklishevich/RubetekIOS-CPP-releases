#pragma once

#include <functional>

#include <msw/int.hpp>
#include <msw/assert.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/plain_traits.hpp>        // byte, wbyte, qbyte
#include <msw/dimension/trigger.hpp>
#include <msw/dimension/counter.hpp>
#include <msw/proto/train/headers.hpp>


namespace msw   {
namespace train {

    struct unwrapper
        : noncopyable
    {
        typedef std::function<void(range<byte const>, qbyte pid, qbyte lid, wbyte ext)> block_ready;

        explicit    unwrapper          (block_ready)                                           ;

        void        add_packet         (range<byte const>)                                     ;

        uint        counter_errors     ()                                                const ;
        uint        protocol_errors    ()                                                const ;

    private:

        void        set_counter_error  ()                                                      ;
        void        set_protocol_error ()                                                      ;
        size<qbyte> block_len          ()                                                const ;
        void        dissect_payload    (size<qbyte> pointer, range<qbyte const> payload)       ;
        bool        unload_wagon       ()                                                      ;

        block_ready    block_ready_     ;
        trigger        sync_            ;
        buffer<qbyte>  wagon_           ;
        counter<wbyte> counter_         ;
        counter<uint>  counter_errors_  ;
        counter<uint>  protocol_errors_ ;
    };

}}


namespace msw   {
namespace train {

    inline unwrapper::unwrapper(block_ready block_ready)
        : block_ready_ ( block_ready                       )
        , wagon_       ( 0, wagon_max_size.fit_in<qbyte>() )
    {}

    inline void unwrapper::add_packet(range<byte const> input_packet)
    {
        if (input_packet.size().not_even(size<byte>(4)) || (input_packet.size() < size<byte>(8)))
            set_protocol_error(); // packet_size_not_multiple_of_4_bytes, packet_size_less_than_8_bytes
        else
        {
            packet_header const& header = input_packet.pop_front<packet_header>();
            range<qbyte const> payload = input_packet.all<qbyte>();

            wbyte const field_counter = header.counter;
            if (sync_ && (field_counter != counter_.value())) set_counter_error(); // counter_error

            wbyte const field_pointer = header.pointer;
            if (size<byte>(field_pointer) == wagon_max_size)
            {
                if (sync_)
                {
                    if (wagon_.empty())
                    {
                        set_protocol_error(); // monopole_packet_without_beginning
                    }
                    else
                    {
                        if (block_len() >= (wagon_.size() - 1 + payload.size()))
                        {
                            wagon_.push_back_silent(payload);
                            if (block_len() == (wagon_.size() - 1)) unload_wagon();
                        }
                        else
                        {
                            set_protocol_error(); // received_more_data_than_in_block_size
                        }
                    }
                }
            }
            else
            {
                if (field_pointer % 4)
                    set_protocol_error(); // pointer_not_multiple_of_4_bytes
                else
                {
                    size<qbyte> ptr = field_pointer / 4;
                    if (ptr > payload.size()) // last fix: was '<='
                        set_protocol_error(); // pointer_beyond_border_packet
                    else
                        dissect_payload(ptr, payload);
                }
            }

            counter_.set(field_counter + 1);
        }
    }

    inline uint unwrapper::counter_errors() const
    {
        return counter_errors_.value();
    }
    inline uint unwrapper::protocol_errors() const
    {
        return protocol_errors_.value();
    }

    inline void unwrapper::set_counter_error()
    {
        ++counter_errors_;
        wagon_.clear();
        sync_.reset();
    }
    inline void unwrapper::set_protocol_error()
    {
        ++protocol_errors_;
        wagon_.clear();
        sync_.reset();
    }
    inline size<qbyte> unwrapper::block_len() const
    {
        return size<byte>(wagon_.all().front<wbyte>()).fit_in<qbyte>();
    }
    inline void unwrapper::dissect_payload(size<qbyte> ptr, range<qbyte const> payload)
    {
        while (payload.has_items())
        {
            if (wagon_.empty())
            {
                if (ptr.not_null())
                {
                    if (sync_) set_protocol_error(); // not_null_pointer_at_empty_wagon
                    payload.pop_front(ptr);
                }
                if (payload.has_items())
                {
                    wagon_.push_back_silent(payload.pop_front());
                    wagon_.push_back_silent(payload.pop_front_max(block_len()));
                    if (wagon_.size() == (block_len() + 1))
                        if (!unload_wagon()) break;
                }
            }
            else
            {
                if (block_len() == ((wagon_.size() - 1) + ptr))
                {
                    wagon_.push_back_silent(payload.pop_front(ptr));
                    if (!unload_wagon()) break;
                }
                else
                {
                    set_protocol_error(); // block_size_not_match_wagon
                    break;
                }
            }
            ptr.clear();
        }
    }
    inline bool unwrapper::unload_wagon()
    {
        sync_.set();
        if (wagon_.size() < size<wagon_header>(1)) return false;
        range<byte const> wagon_rng = wagon_.all<byte>();
        wagon_header const& header = wagon_rng.pop_front<wagon_header>();
        size<byte> len(header.len);
        if (len < size<byte>(8))
        {
            set_protocol_error(); // block_size_less_than_8_bytes
            return false;
        }
        len -= 8;
        if (len > wagon_rng.size()) return false;
        block_ready_(wagon_rng.crop_front(len), header.pid, header.lid, header.ext);
        wagon_.clear();
        return true;
    }

}}
