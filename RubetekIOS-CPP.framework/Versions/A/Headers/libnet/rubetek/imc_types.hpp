#pragma once

#include <msw/config.hpp>

#ifdef MSW_MSVC
#	include <msw/include/windows.hpp> // <Winsock2.h>
#endif

#ifdef MSW_MSVC
#   pragma warning(push)
#       pragma warning(disable:4067)
#       ifdef ERROR
#           define RUBETEK_WINSOCK2_ERROR ERROR ;
#           undef ERROR;
#       endif
#   pragma warning(pop)
#endif

#include "imc_constants.h" // ERROR

#ifdef MSW_MSVC
#   pragma warning(push)
#      pragma warning(disable:4067)
#      ifdef RUBETEK_WINSOCK2_ERROR
#          define ERROR RUBETEK_WINSOCK2_ERROR;
#          undef RUBETEK_WINSOCK2_ERROR;
#      endif
#   pragma warning(pop)
#endif
