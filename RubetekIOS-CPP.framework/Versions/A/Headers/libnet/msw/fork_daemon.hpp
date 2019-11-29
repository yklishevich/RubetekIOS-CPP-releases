#pragma once

#include <stdexcept>

#include <unistd.h>


namespace msw
{
    inline bool fork_daemon()
    {
        pid_t pid = ::fork();
        if (pid == -1) throw std::runtime_error("fork daemon failed");
        else if (!pid)
        {
            if (::setsid() > 0) return true;
            else throw std::runtime_error("set session failed");
        }
        return false;
    }
}
