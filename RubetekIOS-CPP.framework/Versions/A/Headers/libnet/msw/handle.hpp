#pragma once

#include <msw/range.hpp>
#include <msw/config.hpp>
#ifdef MSW_WINAPI
#   include <msw/include/windows.hpp>
#else
#   include <fcntl.h>
#   include <unistd.h>
#endif
#include <msw/system_error/throw.hpp>


namespace msw
{
    struct handle
        : noncopyable
    {
        typedef
#ifdef MSW_WINAPI
            ::HANDLE
#else
            int
#endif
                native_type;

        handle()
            : h_(0)
        {}
        explicit handle(native_type h)
            : h_(h)
        {}
        ~handle()
        {
            close();
        }
        handle(handle&& other)
            : h_(other.release())
        {}

        handle& operator = (handle&& other)
        {
            close();
            h_ = other.release();
            return *this;
        }

        explicit operator bool() const
        {
            return h_ != 0;
        }

        void close()
        {
            if (h_)
            {
#ifdef MSW_WINAPI
                ::CloseHandle(h_);
#else
                ::close(h_);
#endif
                h_ = 0;
            }
        }
        native_type release()
        {
            native_type h = h_;
            h_ = 0;
            return h;
        }

        native_type native() const
        {
            MSW_ASSERT(h_);
            return h_;
        }

#ifndef MSW_WINAPI
        void set_option(int opt)
        {
            if (::fcntl(native(), F_SETFD, opt) == -1) system_error::throw_exception("set option failed");
        }
#endif

    private:
        native_type h_;
    };

    inline void read(handle const& h, range<byte>& block, bool& error)
    {
#ifdef MSW_WINAPI
        unsigned long readed_bytes = 0;
        error = ::ReadFile(h.native(), block.data().native(), static_cast<uint32>(block.size().count()), &readed_bytes, NULL) != TRUE;
#else
        int const readed_bytes = ::read(h.native(), block.data().native(), block.size().count());
        error = readed_bytes == -1;
#endif
        block.crop_front(readed_bytes);
    }
    inline range<byte> read(handle const& h, range<byte> block)
    {
        bool err;
        read(h, block, err);
        if (err) system_error::throw_exception("read failed");
        return block;
    }

    inline void write(handle const& h, range<byte const>& block, bool& error)
    {
#ifdef MSW_WINAPI
        unsigned long written_bytes = 0;
        error = ::WriteFile(h.native(), block.data().native(), static_cast<uint32>(block.size().count()), &written_bytes, NULL) != TRUE;
#else
        int const written_bytes = ::write(h.native(), block.data().native(), block.size().count());
        error = written_bytes == -1;
#endif
        if (!error) error = size<byte>(written_bytes) != block.size();
    }
    inline void write(handle const& h, range<byte const> block)
    {
        bool err;
        write(h, block, err);
        if (err) system_error::throw_exception("write failed");
    }
}
