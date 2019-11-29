#pragma once

#ifndef ZIP_EXTERN
#   define ZIP_EXTERN
#endif

#include <zip.h>

#include <msw/config.hpp>

#ifdef MSW_MSVC
#   ifndef LAMA_DISABLE_LIBZIP_AUTOLINK
#       include <lama/libzip/include/autolink.hpp>
#   endif
#endif
