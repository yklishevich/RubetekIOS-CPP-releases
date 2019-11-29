#pragma once

#include <msw/config.hpp>

#ifdef MSW_MSVC
#   pragma warning (push)
#   pragma warning (disable: 4100)
#   pragma warning (disable: 4503)
#   pragma warning (disable: 4512)
#   pragma warning (disable: 4715)
#endif

#include <boost/property_tree/json_parser.hpp>

#ifdef MSW_MSVC
#   pragma warning(pop)
#endif
