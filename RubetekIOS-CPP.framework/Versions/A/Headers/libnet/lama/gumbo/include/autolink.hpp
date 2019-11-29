#pragma once

#include <msw/config.hpp>


#if _MSC_VER == 1900
    #ifdef MSW_RUNTIME_DEBUG_SHARED
        #pragma comment(lib, "libgumbo-vc140-d.lib")
    #elif defined MSW_RUNTIME_DEBUG_STATIC
        #pragma comment(lib, "libgumbo-vc140-sd.lib")
    #elif defined MSW_RUNTIME_RELEASE_SHARED
        #pragma comment(lib, "libgumbo-vc140.lib")
    #elif defined MSW_RUNTIME_RELEASE_STATIC
        #pragma comment(lib, "libgumbo-vc140-s.lib")
    #endif
#endif

#if _MSC_VER == 1800
    #ifdef MSW_RUNTIME_DEBUG_SHARED
        #pragma comment(lib, "libgumbo-vc120-d.lib")
    #elif defined MSW_RUNTIME_DEBUG_STATIC
        #pragma comment(lib, "libgumbo-vc120-sd.lib")
    #elif defined MSW_RUNTIME_RELEASE_SHARED
        #pragma comment(lib, "libgumbo-vc120.lib")
    #elif defined MSW_RUNTIME_RELEASE_STATIC
        #pragma comment(lib, "libgumbo-vc120-s.lib")
    #endif
#endif
