#pragma once 

#include <string>

#include <msw/config.hpp>
#ifdef MSW_WINAPI
#   include <msw/include/windows.hpp>
#   include <msw/command_line/build_string.hpp>
#else
#   include <unistd.h>
#   include <signal.h>
#   include <sys/resource.h>
#   include <msw/create_pipe.hpp>
#   include <msw/lexical_cast/bytes_as_string.hpp>
#endif
#include <msw/process_info.hpp>
#include <msw/system_error/throw.hpp>
#include <msw/system_error/last_message.hpp>


namespace msw {
namespace zzz {

#ifndef MSW_WINAPI
    namespace lin
    {
        inline void close_all_handles()
        {
            struct rlimit rl;
            getrlimit(RLIMIT_NOFILE, &rl);
            for (unsigned i = 0; i != rl.rlim_max; ++i)
                close(i);
        }
    }
#endif

    template <typename ...Args>
    process_info execute_process
    (
          std::string&                                      error
        , std::function<void(MSW_ON_WINAPI(STARTUPINFOA&))> pre_execute
        , bool                                              win_inherit_handles
        , char const*                                       path
        , Args&&...                                         args
    )
    {
#ifdef MSW_WINAPI
        STARTUPINFOA        startup_info ;
        PROCESS_INFORMATION proc_info    ;
        memset(&startup_info, 0, sizeof(startup_info));
        memset(&proc_info,    0, sizeof(proc_info)   );
        startup_info.cb = sizeof(startup_info);
        if (pre_execute) pre_execute(startup_info);
        std::string const cmd_line = build_command_line_string(path, std::forward<Args>(args)...);
        if (!::CreateProcessA
            (
                0
                , const_cast<char*>(cmd_line.c_str())
                , 0, 0, win_inherit_handles, 0, 0, 0, &startup_info, &proc_info
            ))
        {
            error = system_error::last_message();
        }
        return process_info({proc_info.hProcess, static_cast<msw::int32>(proc_info.dwProcessId)});
#else
        handle h_in  ;
        handle h_out ;
        create_pipe(h_in, h_out);
        h_in .set_option(FD_CLOEXEC);
        h_out.set_option(FD_CLOEXEC);
        ::signal(SIGCHLD, SIG_IGN);
        int const pid = ::fork();
        if (pid == 0)
        {
            if (pre_execute) pre_execute();
            lin::close_all_handles();
            ::execlp(path, path, std::forward<Args>(args)..., nullptr);
            write(h_out, make_range<byte>(system_error::last_message()));
            ::exit(0);
        }
        else if (pid == -1)
        {
            error = system_error::last_message();
        }
        else
        {
            h_out.close();
            buffer<byte> buf(KB);
            error = bytes_as_string(read(h_in, buf.all()));
        }
        return process_info({pid, pid});
#endif
    }

}}


namespace msw
{
    template <typename ...Args>
    process_info execute_process_ex(std::function<void(MSW_ON_WINAPI(STARTUPINFOA&))> pre_execute, bool win_inherit_handles, std::string const& path, Args ...args)
    {
        std::string err;
        process_info const info = zzz::execute_process(err, pre_execute, win_inherit_handles, path.c_str(), std::string(args).c_str()...);
        if (!err.empty()) throw_runtime_error("can't execute process '" + path + "': ", err);
        return info;
    }
    template <typename ...Args>
    process_info execute_process(std::string const& path, Args&& ...args)
    {
        return execute_process_ex(nullptr, false, path, std::forward<Args>(args)...);
    }
}
