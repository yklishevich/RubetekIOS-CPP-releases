#pragma once

#include <msw/config.hpp>

#ifdef MSW_WINDOWS
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   ifndef NOMINMAX
#       define NOMINMAX
#   endif
#   include "winsock2.h"
#   include "windows.h"
#else
#   error "windows.hpp only for windows system"
#endif
