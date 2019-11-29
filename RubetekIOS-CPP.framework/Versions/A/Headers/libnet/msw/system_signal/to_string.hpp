#pragma once

#include <string>

#include <msw/config.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw
{
    inline char const* system_signal_to_string(system_signal s)
    {
        switch (s)
        {
#define MSW_SYSTEM_SIGNAL_TO_STRING_CASE(N) case system_signal::N: return #N;
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( none           )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( cancel         )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( stop           )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( close          )

            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_hup        )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_int        )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_quit       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_ill        )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_trap       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_abrt       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_fpe        )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_bus        )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_segv       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_sys        )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_pipe       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_alrm       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_term       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_usr1       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_usr2       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_chld       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_tstp       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_urg        )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_poll       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_cont       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_ttin       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_ttou       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_vtalrm     )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_prof       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_xcpu       )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( uni_xfsz       )

            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( win_ctrl_c     )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( win_ctrl_break )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( win_close      )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( win_logoff     )
            MSW_SYSTEM_SIGNAL_TO_STRING_CASE( win_shutdown   )
#undef MSW_SYSTEM_SIGNAL_TO_STRING_CASE
        };
        throw_runtime_error("can't convert system signal (", static_cast<int>(s), ") to string");
#include <msw/throw_hide_warning.hpp>
    }
}
