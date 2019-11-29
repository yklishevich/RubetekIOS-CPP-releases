#pragma once

#include <msw/handle.hpp>
#include <msw/system_error/throw.hpp>

#include <dawn/nix/timeval_cast.hpp>


namespace dawn {
namespace nix  {

    template <typename Duration>
    bool select_read(handle const& h, Duration timeout, bool& error)
    {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(h.native(), &rfds);
        timeval tv = timeval_from_chrono<timeval>(timeout);
        int ret = ::select(h.native() + 1, &rfds, 0, 0, &tv);
        error = ret == -1;
        return ret > 0;
    }
    template <typename Duration>
    bool select_read(handle const& h, Duration timeout)
    {
        bool err;
        bool const ret = select_read(h, timeout, err);
        if (err) system_error::throw_exception("select read failed");
        return ret;
    }

}}
