#pragma once

#include <msw/config.hpp>

#ifdef MSW_MSVC
#	pragma warning(push)
#	pragma warning(disable:4100)
#	pragma warning(disable:4267)
#	pragma warning(disable:4458)
#	pragma warning(disable:4800)
#endif

#include <beast/websocket.hpp>

#ifdef MSW_MSVC
#	pragma warning(pop)
#endif
