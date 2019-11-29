#pragma once 

#include <limits>
#include <exception>
#include <functional>
#include <type_traits>

#include <msw/config.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw {
namespace spp {

    template <typename SizeType = wbyte>
    struct packer
        : noncopyable
    {
        typedef SizeType                         size_type    ;
        typedef std::function<void(range<byte>)> packet_ready ;

        static_assert(std::numeric_limits<size_type>::is_integer && std::is_unsigned<size_type>::value, "SizeType must be only unsigned integer type");

        explicit packer(packet_ready packet_ready, size<byte> buffer_capacity = msw::KB * 64, size<byte> offset = 0)
            : offset_          ( offset             )
            , buf_             ( 0, buffer_capacity )
            , last_packet_len_ ( 0                  )
            , packet_ready_    ( packet_ready       )
        {}
        ~packer()
        {
            if (!std::uncaught_exception()) flush();
        }

        template <typename ...T>
        void add_packet(T&& ...v)
        {
            flush();
            put_packet(std::forward<T>(v)...);
            zzz_flush();
        }

        template <typename ...T>
        void add_packet_silent(T&& ...v)
        {
            flush();
            put_packet_silent(std::forward<T>(v)...);
            zzz_flush();
        }

        template <typename ...Ts>
        void put_packet(Ts&& ...vs)
        {
            zzz_add_header();
            put_to_last_packet(std::forward<Ts>(vs)...);
        }

        template <typename ...Ts>
        void put_packet_silent(Ts&& ...vs)
        {
            zzz_add_header_silent();
            put_to_last_packet_silent(std::forward<Ts>(vs)...);
        }

        void put_to_last_packet(range<byte const> block)
        {
            auto const max_block_size = size<byte>(std::numeric_limits<size_type>::max());
            if ((size<byte>(last_packet_len_) + block.size()) > max_block_size)
                msw::throw_runtime_error("large block: ", last_packet_len_ , " + ", block.size(), " (max permissible size: ", max_block_size, ")");

            if (buf_.empty()) zzz_add_header();
            zzz_add_last_packet_len(static_cast<size_type>(block.size().count()));
            buf_.push_back(block);
        }
        template <typename T>
        void put_to_last_packet(T&& v)
        {
#ifdef MSW_ODD_STD_FORWARD
            put_to_last_packet(make_range<byte const>(v));
#else
            put_to_last_packet(make_range<byte const>(std::forward<T>(v)));
#endif
        }
        template <typename T, typename ...Ts>
        void put_to_last_packet(T&& v, Ts&& ...vs)
        {
            put_to_last_packet(std::forward<T>(v));
            put_to_last_packet(std::forward<Ts>(vs)...);
        }

        void put_to_last_packet_silent(range<byte const> block)
        {
            MSW_ASSERT((size<byte>(last_packet_len_) + block.size()) <= size<byte>(std::numeric_limits<size_type>::max()));
            if (buf_.empty()) zzz_add_header_silent();
            zzz_add_last_packet_len(static_cast<size_type>(block.size().count()));
            buf_.push_back_silent(block);
        }
        template <typename T>
        void put_to_last_packet_silent(T&& v)
        {
            put_to_last_packet_silent(make_range<byte const>(std::forward<T>(v)));
        }
        template <typename T, typename ...Ts>
        void put_to_last_packet_silent(T&& v, Ts&& ...vs)
        {
            put_to_last_packet_silent(std::forward<T>(v));
            put_to_last_packet_silent(std::forward<Ts>(vs)...);
        }

        size<byte> packet_size() const
        {
            return buf_.size();
        }
        size<byte> capacity() const
        {
            return buf_.capacity();
        }
        size<byte> offset() const
        {
            return offset_;
        }
        bool empty() const
        {
            return buf_.empty();
        }
        void flush()
        {
            if (buf_.has_items()) zzz_flush();
        }
        void reset()
        {
            buf_.clear();
            last_packet_len_ = 0;
        }
    private:

        void zzz_flush()
        {
            packet_ready_(buf_.all());
            last_packet_len_ = 0;
            buf_.clear();
        }
        void zzz_add_header()
        {
            if (buf_.empty()) buf_.resize(offset_);
            buf_.push_back(size_type(0));
            last_packet_len_ = 0;
        }
        void zzz_add_header_silent()
        {
            if (buf_.empty()) buf_.resize(offset_);
            buf_.push_back_silent(size_type(0));
            last_packet_len_ = 0;
        }
        void zzz_add_last_packet_len(size_type len)
        {
            size_type& packet_len = buf_.back(last_packet_len_ + sizeof(size_type)).template front<size_type>();
            last_packet_len_ += len;
            packet_len += len;
            MSW_ASSERT(packet_len == last_packet_len_);
        }

        size<byte>   const offset_          ;
        buffer<byte>       buf_             ;
        size_type          last_packet_len_ ;
        packet_ready       packet_ready_    ;
    };

    typedef packer<byte>  packer_8  ;
    typedef packer<wbyte> packer_16 ;
    typedef packer<qbyte> packer_32 ;

}}
