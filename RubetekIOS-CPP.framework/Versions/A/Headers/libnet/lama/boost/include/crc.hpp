#pragma once

#include <msw/config.hpp>

#ifdef MSW_MSVC
#   pragma warning(push)
#   pragma warning(disable:4244)
#   pragma warning(disable:4245)
#   pragma warning(disable:4458)
#   pragma warning(disable:4800)
#endif

#include <boost/crc.hpp>

#ifdef MSW_MSVC
#   pragma warning(pop)
#endif
