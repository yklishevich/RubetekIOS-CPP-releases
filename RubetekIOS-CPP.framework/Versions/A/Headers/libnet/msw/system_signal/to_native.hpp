#pragma once

#include <string>
#include <stdexcept>

#include <msw/config.hpp>
#ifdef MSW_LINUX
#   include <signal.h>
#else
#   include <msw/include/windows.hpp>
#endif
#include <msw/throw_runtime_error.hpp>
#include <msw/system_signal/signal.hpp>


namespace msw
{
    inline int system_signal_to_native(system_signal s)
    {
#ifdef MSW_GCC
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wswitch"
#endif
        switch (s)
        {
#define MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE(S, N) case system_signal::S: return N;
#   ifdef MSW_LINUX
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_hup        , SIGHUP              );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_int        , SIGINT              );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_quit       , SIGQUIT             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_ill        , SIGILL              );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_trap       , SIGTRAP             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_abrt       , SIGABRT             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_fpe        , SIGFPE              );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_bus        , SIGBUS              );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_segv       , SIGSEGV             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_sys        , SIGSYS              );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_pipe       , SIGPIPE             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_alrm       , SIGALRM             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_term       , SIGTERM             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_usr1       , SIGUSR1             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_usr2       , SIGUSR2             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_chld       , SIGCHLD             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_tstp       , SIGTSTP             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_urg        , SIGURG              );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_poll       , SIGPOLL             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_cont       , SIGCONT             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_ttin       , SIGTTIN             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_ttou       , SIGTTOU             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_vtalrm     , SIGVTALRM           );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_prof       , SIGPROF             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_xcpu       , SIGXCPU             );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( uni_xfsz       , SIGXFSZ             );
#   else
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( win_ctrl_c     , CTRL_C_EVENT        );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( win_ctrl_break , CTRL_BREAK_EVENT    );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( win_close      , CTRL_CLOSE_EVENT    );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( win_logoff     , CTRL_LOGOFF_EVENT   );
            MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE( win_shutdown   , CTRL_SHUTDOWN_EVENT );
#   endif
#undef MSW_SYSTEM_SIGNAL_TO_NATIVE_CASE
        };
#ifdef MSW_GCC
#   pragma GCC diagnostic pop
#endif
        throw_runtime_error("can't convert system signal (", static_cast<int>(s), ") to native");
#include <msw/throw_hide_warning.hpp>
    }
}
