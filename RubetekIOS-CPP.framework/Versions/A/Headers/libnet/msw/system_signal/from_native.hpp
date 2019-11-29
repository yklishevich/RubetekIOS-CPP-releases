#pragma once

#include <string>

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
    inline system_signal system_signal_from_native(int s)
    {
        switch (s)
        {
#define MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE(N, S) case N: return system_signal::S;
#   ifdef MSW_LINUX
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGHUP              , uni_hup        );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGINT              , uni_int        );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGQUIT             , uni_quit       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGILL              , uni_ill        );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGTRAP             , uni_trap       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGABRT             , uni_abrt       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGFPE              , uni_fpe        );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGBUS              , uni_bus        );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGSEGV             , uni_segv       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGSYS              , uni_sys        );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGPIPE             , uni_pipe       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGALRM             , uni_alrm       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGTERM             , uni_term       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGUSR1             , uni_usr1       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGUSR2             , uni_usr2       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGCHLD             , uni_chld       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGTSTP             , uni_tstp       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGURG              , uni_urg        );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGPOLL             , uni_poll       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGCONT             , uni_cont       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGTTIN             , uni_ttin       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGTTOU             , uni_ttou       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGVTALRM           , uni_vtalrm     );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGPROF             , uni_prof       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGXCPU             , uni_xcpu       );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( SIGXFSZ             , uni_xfsz       );
#   else
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( CTRL_C_EVENT        , win_ctrl_c     );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( CTRL_BREAK_EVENT    , win_ctrl_break );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( CTRL_CLOSE_EVENT    , win_close      );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( CTRL_LOGOFF_EVENT   , win_logoff     );
            MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE( CTRL_SHUTDOWN_EVENT , win_shutdown   );
#   endif
#undef MSW_SYSTEM_SIGNAL_FROM_NATIVE_CASE
        };
        throw_runtime_error("can't convert native signal (", s, ") to system");
#include <msw/throw_hide_warning.hpp>
    }
}
