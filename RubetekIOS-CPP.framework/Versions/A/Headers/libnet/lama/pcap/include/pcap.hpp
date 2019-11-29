#pragma once

#ifdef MSW_WINDOWS
#   ifndef WIN32
#       define WIN32
#   endif
#endif

#define WPCAP
#define HAVE_REMOTE

#include <msw/config.hpp>

#ifdef MSW_MSVC
#   define NOMINMAX
#   include <xkeycheck.h>
#   include <inttypes.h>  //for remove warning C4005: 'PRId64': macro redefinition (when using Qt)
#endif

#include <pcap.h>

#ifdef MSW_MSVC
#   ifndef LAMA_DISABLE_PCAP_AUTOLINK
#       include <lama/pcap/include/autolink.hpp>
#   endif
#endif
