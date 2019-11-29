#pragma once

#include <set>

#include <unistd.h>
#include <signal.h>

#include <msw/std/chrono.hpp>
#include <msw/system_error/throw.hpp>


namespace msw
{
    struct uni_signal_catcher
    {
        explicit uni_signal_catcher (std::set<int> const& signals, chrono::steady_clock::duration timeout = chrono::milliseconds(1)) ;
        int      wait_signal        ()                                                                                               ;
    private:
        sigset_t sigset_ ;
        timespec tm_     ;
    };
};


namespace msw
{
    inline uni_signal_catcher::uni_signal_catcher(std::set<int> const& signals, chrono::steady_clock::duration timeout)
    {
        chrono::milliseconds const mlsecs =  chrono::duration_cast<chrono::milliseconds>(timeout);
        tm_.tv_sec = mlsecs.count() / 1000;
        tm_.tv_nsec = (mlsecs.count() % 1000) * 1000000;
        if (::sigemptyset(&sigset_)) system_error::throw_exception("sigemptyset fail");
        for (int s : signals)
            if (::sigaddset(&sigset_, s)) system_error::throw_exception("sigaddset fail");
        if (::sigprocmask(SIG_BLOCK, &sigset_, 0)) system_error::throw_exception("sigprocmask fail");
    }

    inline int uni_signal_catcher::wait_signal()
    {
        return ::sigtimedwait(&sigset_, 0, &tm_);
    }
};
