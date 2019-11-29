#pragma once

#include <msw/config.hpp>
#include <msw/include/windows.hpp>

#ifdef MSW_WINDOWS
#   define SECURITY_WIN32
#   include <Wincrypt.h>
#   include <Security.h>
#   include <Schannel.h>
#else
#   error "schannel.hpp only for windows system"
#endif
