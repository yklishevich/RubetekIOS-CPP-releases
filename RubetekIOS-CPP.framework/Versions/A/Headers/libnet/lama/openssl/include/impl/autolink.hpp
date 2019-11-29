#pragma once


#ifdef MSW_RUNTIME_DEBUG_SHARED
    #pragma comment(lib, "libeay32MDd.lib")
    #pragma comment(lib, "ssleay32MDd.lib")
#elif defined MSW_RUNTIME_DEBUG_STATIC
    #pragma comment(lib, "libeay32MTd.lib")
    #pragma comment(lib, "ssleay32MTd.lib")
#elif defined MSW_RUNTIME_RELEASE_SHARED
    #pragma comment(lib, "libeay32MD.lib")
    #pragma comment(lib, "ssleay32MD.lib")
#elif defined MSW_RUNTIME_RELEASE_STATIC
    #pragma comment(lib, "libeay32MT.lib")
    #pragma comment(lib, "ssleay32MT.lib")
#endif
