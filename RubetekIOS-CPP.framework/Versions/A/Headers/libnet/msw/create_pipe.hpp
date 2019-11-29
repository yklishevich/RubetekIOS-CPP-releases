#pragma once

#include <msw/config.hpp>
#include <msw/handle.hpp>
#include <msw/system_error/throw.hpp>


namespace msw
{
    inline void create_pipe(handle& in, handle& out, bool& err, bool MSW_ON_WINAPI(win_inherit_handle = false))
    {
#ifdef MSW_WINAPI
        SECURITY_ATTRIBUTES sec_attrs;
        if (win_inherit_handle)
        {
            memset(&sec_attrs, 0, sizeof(sec_attrs));
            sec_attrs.nLength        = sizeof(SECURITY_ATTRIBUTES);
            sec_attrs.bInheritHandle = TRUE;
        }

        HANDLE h_in, h_out;
        err = ::CreatePipe(&h_in, &h_out, win_inherit_handle ? &sec_attrs : nullptr, 0) != TRUE;
        if (!err)
        {
            in  = handle(h_in);
            out = handle(h_out);
        }
#else
        int pipes[2];
        err = ::pipe(pipes) != 0;
        if (!err)
        {
            in  = handle(pipes[0]);
            out = handle(pipes[1]);
        }
#endif
    }
    inline void create_pipe(handle& in, handle& out, bool win_inherit_handle = false)
    {
        bool err;
        create_pipe(in, out, err, win_inherit_handle);
        if (err) system_error::throw_exception("create pipe failed");
    }
}
