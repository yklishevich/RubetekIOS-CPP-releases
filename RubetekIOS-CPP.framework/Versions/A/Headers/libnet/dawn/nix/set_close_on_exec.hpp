#pragma once

#include <fcntl.h>


namespace dawn {
namespace nix  {

    inline void set_close_on_exec(int fd)
    {
        fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
    }

}}
