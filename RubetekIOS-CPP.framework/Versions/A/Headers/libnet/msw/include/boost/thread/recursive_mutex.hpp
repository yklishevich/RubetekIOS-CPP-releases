#pragma once

#include <msw/config.hpp>

#ifdef MSW_MINGW
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#   pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#include <boost/thread/recursive_mutex.hpp>

#ifdef MSW_MINGW
#   pragma GCC diagnostic pop
#endif
