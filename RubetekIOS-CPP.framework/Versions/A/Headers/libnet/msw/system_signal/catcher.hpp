#pragma once

#include <set>

#include <msw/std/thread.hpp>
#include <msw/std/chrono.hpp>
#include <msw/system_signal/signal.hpp>
#include <msw/system_signal/to_native.hpp>
#include <msw/system_signal/from_native.hpp>

#ifdef MSW_WINDOWS
#   include <msw/system_signal/win_catcher.hpp>
#else
#   include <msw/system_signal/uni_catcher.hpp>
#endif


namespace msw
{
    struct signal_catcher
    {
                explicit      signal_catcher     (std::set<system_signal> const&, chrono::steady_clock::duration timeout = chrono::milliseconds(1)) ;
                system_signal wait_signal        ()                                                                                                 ;
    private:
                system_signal process_signal     (int)                                                                                              ;
#ifdef MSW_LINUX
        static  std::set<int> signals_to_navitve (std::set<system_signal> const&)                                                                   ;
#endif

        std::set<system_signal> signals_ ;
#ifdef MSW_WINDOWS
        int64                   delay_   ;
        win_signal_catcher      catcher_ ;
#else
        uni_signal_catcher      catcher_ ;
#endif
    };
};


namespace msw
{
    inline signal_catcher::signal_catcher(std::set<system_signal> const& signals, chrono::steady_clock::duration timeout)
        : signals_  (signals)
#ifdef MSW_WINDOWS
        , delay_    (chrono::duration_cast<chrono::milliseconds>(timeout).count())
#else
        , catcher_  (signals_to_navitve(signals))
#endif
    {}

    inline system_signal signal_catcher::wait_signal()
    {
#ifdef MSW_WINDOWS
        for (int64 n = 0; n != delay_; ++n)
        {
            int const s = catcher_.check_signal();
            if (s != -1) return process_signal(s);
            this_thread::sleep_for(chrono::milliseconds(1));
        }
#else
        int const s = catcher_.wait_signal();
        if (s != -1) return process_signal(s);
#endif
        return system_signal::none;
    }

    inline system_signal signal_catcher::process_signal(int s)
    {
        system_signal const ss = system_signal_from_native(s);
        if (signals_.count(ss)) return ss;
        if (signals_.count(system_signal::cancel))
        {
            if
#ifdef MSW_WINDOWS
                (ss == system_signal::win_ctrl_c)
#else
                (ss == system_signal::uni_int)
#endif
                return system_signal::cancel;
        }
        if (signals_.count(system_signal::stop))
        {
            if
#ifdef MSW_WINDOWS
                (ss <= system_signal::win_ctrl_break)
#else
                ((ss == system_signal::uni_quit) || (ss == system_signal::uni_tstp))
#endif
                return system_signal::stop;
        }
        if (signals_.count(system_signal::close)) return system_signal::close;
        ::exit(1);
    }

#ifdef MSW_LINUX
    inline std::set<int> signal_catcher::signals_to_navitve(std::set<system_signal> const& signals)
    {
        std::set<int> native_signals;
        for (system_signal s : signals)
        {
            if (s == system_signal::none) continue;
            else if (s <= system_signal::close)
            {
                native_signals.insert(system_signal_to_native(system_signal::uni_int    ));
                if (s == system_signal::cancel) continue;
                native_signals.insert(system_signal_to_native(system_signal::uni_quit   ));
                native_signals.insert(system_signal_to_native(system_signal::uni_tstp   ));
                if (s == system_signal::stop) continue;
                native_signals.insert(system_signal_to_native(system_signal::uni_hup    ));
                native_signals.insert(system_signal_to_native(system_signal::uni_term   ));
            }
            else if (s < system_signal::win_ctrl_c) native_signals.insert(system_signal_to_native(s));
        }
        return native_signals;
    }
#endif
};
