#pragma once

#include <msw/config.hpp>

#ifdef MSW_MSVC
#   ifndef _WIN32_WINNT
#       define _WIN32_WINNT 0x0602
#   endif
#endif
#ifdef MSW_MINGW
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#       include <boost/bind/arg.hpp>
#   pragma GCC diagnostic pop
#   ifdef MSW_RELEASE
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wstrict-aliasing"
#           include <boost/thread/win32/shared_mutex.hpp>
#           include <boost/asio/detail/impl/win_iocp_handle_service.ipp>
#       pragma GCC diagnostic pop
#   endif
#endif
#ifdef MSW_WONDOWS
#   include #include <msw/include/windows.hpp>
#endif
#ifdef MSW_MSVC
#   pragma warning (disable: 4312)
#endif
#include <boost/asio.hpp>
