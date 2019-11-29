#pragma once

#include <msw/config.hpp>

#ifdef MSW_MINGW
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif
#ifdef MSW_RELEASE
#   ifdef MSW_GCC_4_4
#       pragma GCC diagnostic ignored "-Wstrict-aliasing"
#   endif
#endif

#ifdef MSW_MSVC
#   pragma warning (push)
#   pragma warning (disable: 4503)
#endif

#include <boost/property_tree/ptree.hpp>

#ifdef MSW_MINGW
#   pragma GCC diagnostic pop
#endif

#ifdef MSW_MSVC
//# pragma warning(pop)
#endif
