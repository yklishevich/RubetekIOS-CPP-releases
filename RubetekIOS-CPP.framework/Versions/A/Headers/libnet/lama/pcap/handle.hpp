#pragma once

#include <msw/size.hpp>
#include <msw/config.hpp>
#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>

#include <lama/pcap/stats.hpp>
#include <lama/pcap/include/pcap.hpp>


namespace lama {
namespace pcap {

    struct handle
        : msw::noncopyable
    {
        handle()
            : h_(nullptr)
        {}
        explicit handle(pcap_t* h)
            : h_(h)
        {}
        ~handle()
        {
            close();
        }
        handle(handle&& other)
            : h_(other.h_)
        {
            other.h_ = nullptr;
        }
        handle& operator = (handle&& other)
        {
            close();
            h_ = other.release();
            return *this;
        }

        explicit operator bool() const
        {
            return h_ != nullptr;
        }
        void close()
        {
            if (h_)
            {
                ::pcap_close(h_);
                h_ = nullptr;
            }
        }
        pcap_t* release()
        {
            pcap_t* h = h_;
            h_ = nullptr;
            return h;
        }
        pcap_t* native() const
        {
            MSW_ASSERT(h_);
            return h_;
        }

        void get_stats(stats& s)
        {
            MSW_ASSERT(h_);
            if (::pcap_stats(native(), reinterpret_cast<pcap_stat*>(&s))) msw::throw_runtime_error("error getting pcap statistics: ", ::pcap_geterr(native()));
        }
        void set_buffer_size(msw::size<msw::byte> s)
        {
            MSW_ASSERT(h_);
#ifdef MSW_WINDOWS
            if (::pcap_setbuff(native(), static_cast<msw::uint32>(s.count()))) msw::throw_runtime_error("error set adapter buffer size: ", s, " B");
#endif
        }
    private:
        pcap_t* h_;
    };

}}
