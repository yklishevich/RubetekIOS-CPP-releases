#pragma once


namespace msw
{
    enum class system_signal : int
    {
          none
        , cancel
        , stop
        , close

        , uni_hup        = 32
        , uni_int
        , uni_quit
        , uni_ill
        , uni_trap
        , uni_abrt
        , uni_fpe
        , uni_bus
        , uni_segv
        , uni_sys
        , uni_pipe
        , uni_alrm
        , uni_term
        , uni_usr1
        , uni_usr2
        , uni_chld
        , uni_tstp
        , uni_urg
        , uni_poll
        , uni_cont
        , uni_ttin
        , uni_ttou
        , uni_vtalrm
        , uni_prof
        , uni_xcpu
        , uni_xfsz

        , win_ctrl_c     = 256
        , win_ctrl_break
        , win_close
        , win_logoff     = win_ctrl_c + 5
        , win_shutdown
    };
}
