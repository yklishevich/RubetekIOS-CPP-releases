#pragma once

#include <msw/config.hpp>


#ifdef MSW_MSVC
#   ifndef LAMA_DISABLE_OPENSSL_AUTOLINK
#       include <lama/openssl/include/impl/autolink.hpp>
#   endif
#endif
