#pragma once

#include <msw/config.hpp>


#ifdef MSW_DEBUG
#   define MSW_ON_DEBUG(...) __VA_ARGS__
#else
#   define MSW_ON_DEBUG(...)
#endif
