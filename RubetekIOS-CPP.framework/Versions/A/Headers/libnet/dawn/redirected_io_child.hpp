#pragma once

#include <string>

#include <msw/config.hpp>
#include <msw/buffer.hpp>
#include <msw/handle.hpp>
#include <msw/std/chrono.hpp>
#include <msw/std/thread.hpp>
#include <msw/create_pipe.hpp>
#include <msw/algorithm/copy.hpp>
#include <msw/execute_process.hpp>
#include <msw/algorithm/search.hpp>
#include <msw/terminate_process.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>
#ifdef MSW_WINAPI
#   include <msw/include/windows.hpp>
#else
#   include <dawn/nix/select_read.hpp>
#endif


namespace dawn
{
    struct redirected_io_child
    {
        template <typename ...Args>
        explicit redirected_io_child(std::string const& command, Args&&... args)
            MSW_ON_WINAPI(: last_is_0d_(false))
        {
            handle h_in;
            handle h_out;
            create_pipe(h_in_, h_out , true);
            create_pipe(h_in , h_out_, true);
            process_info_ = msw::execute_process_ex
            (
                [&](MSW_ON_WINAPI(STARTUPINFOA& startup_info))
                {
#ifdef MSW_WINAPI
                    startup_info.dwFlags     = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
                    startup_info.wShowWindow = SW_HIDE;
                    startup_info.hStdOutput  = h_out.native();
                    startup_info.hStdError   = h_out.native();
                    startup_info.hStdInput   = h_in.native();
#else
                    ::dup2(h_in.native() , STDIN_FILENO );
                    ::dup2(h_out.native(), STDOUT_FILENO);
                    ::dup2(h_out.native(), STDERR_FILENO);
#endif
                }
                , true, command, std::forward<Args>(args)...
            );
        }

        msw::range<msw::byte> read(msw::range<msw::byte> block)
        {
#ifdef MSW_WINAPI
            bool err;
            msw::read(h_in_, block, err);
            if (err)
            {
                if (msw::system_error::last_code() == ERROR_BROKEN_PIPE) block.shrink_forward();
                else msw::system_error::throw_exception("read pipe failed");
            }
            if (block.has_items()) win_process_out(block);
#else
            block = msw::read(h_in_, block);
#endif
            if (block.empty()) close();
            return block;
        }
        template <typename Duration>
        bool read(msw::range<msw::byte>& block, Duration timeout)
        {
            bool ret = false;
#ifdef MSW_WINAPI
            uint64 n = chrono::duration_cast<chrono::milliseconds>(timeout).count() + 1;
            for (;;)
            {
                ULONG total_bytes_availible = 0;
                ULONG x = 0;
                if (!::PeekNamedPipe(h_in_.native(), 0, 0, 0, &total_bytes_availible, &x))
                {
                    if (system_error::last_code() == ERROR_BROKEN_PIPE)
                    {
                        block.shrink_forward();
                        ret = true;
                        close();
                        break;
                    }
                    else system_error::throw_exception("peek pipe failed");
                }
                if (total_bytes_availible)
                {
                    block = read(block);
                    ret = true;
                    break;
                }
                if (!--n) break;
                this_thread::sleep_for(chrono::milliseconds(1));
            }
#else
            ret = nix::select_read(h_in_, timeout);
            if (ret) block = read(block);
#endif
            return ret;
        }
        std::string read(msw::size<msw::byte> len)
        {
            msw::buffer<msw::byte> buf(len);
            return msw::bytes_as_string(read(buf.all()));
        }
        template <typename Duration>
        bool read(std::string& s, msw::size<msw::byte> len, Duration timeout)
        {
            buffer<byte> buf(len);
            msw::range<msw::byte> out = buf.all();
            bool const ret = read(out, timeout);
            s = bytes_as_string(out);
            return ret;
        }

        void write(msw::range<msw::byte const> block)
        {
            MSW_ON_WINAPI(win_process_in(block));
            msw::write(h_out_, block);
            MSW_ON_WINAPI(buf_.clear());
        }
        void write(std::string const& s)
        {
            write(msw::make_range<msw::byte>(s));
        }

        bool is_active() const
        {
            return static_cast<bool>(h_in_);
        }

        void terminate()
        {
            msw::terminate_process(process_info_.handle);
        }

    private:

        void close() const
        {
            h_in_.close();
            h_out_.close();
        }

#ifdef MSW_WINAPI
        static msw::byte const r = '\r'; //0d
        static msw::byte const n = '\n'; //0a

        void win_process_in(msw::range<msw::byte const>&)
        {
            // insert 0d before 0a
        }
        void win_process_out(msw::range<msw::byte>& block)
        {
            msw::range<msw::byte> out = block;
            while (out.has_items())
            {
                if (last_is_0d_)
                {
                    if (out.front() != n) buf_.push_back(r);
                    last_is_0d_ = false;
                }
                msw::range<msw::byte>::position const pos = msw::search(r, out);
                if (pos == out.end())
                {
                    buf_.push_back(out);
                    break;
                }
                else
                {
                    last_is_0d_ = true;
                    buf_.push_back(out.pop_front(pos - out.begin()));
                    out.pop_front();
                }
            }
            copy(buf_, block);
            block.crop_front(buf_.size());
            buf_.clear();
        }
#endif

                       msw::handle            mutable h_in_         ;
                       msw::handle            mutable h_out_        ;
                       msw::process_info              process_info_ ;
        MSW_ON_WINAPI( bool                           last_is_0d_ ) ;
        MSW_ON_WINAPI( msw::buffer<msw::byte>         buf_        ) ;
    };
}
