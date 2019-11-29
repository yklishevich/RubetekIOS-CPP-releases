#pragma once


#if _MSC_VER == 1900 
	#ifdef MSW_RUNTIME_DEBUG_SHARED
		#pragma comment(lib, "libzip-vc140-d.lib")
	#elif defined MSW_RUNTIME_DEBUG_STATIC
		#pragma comment(lib, "libzip-vc140-sd.lib")
	#elif defined MSW_RUNTIME_RELEASE_SHARED
		#pragma comment(lib, "libzip-vc140.lib")
	#elif defined MSW_RUNTIME_RELEASE_STATIC
		#pragma comment(lib, "libzip-vc140-s.lib")
	#endif
#endif

#if _MSC_VER == 1800 
	#ifdef MSW_RUNTIME_DEBUG_SHARED
		#pragma comment(lib, "libzip-vc120-debug-rt=shared.lib")
	#elif defined MSW_RUNTIME_DEBUG_STATIC
		#pragma comment(lib, "libzip-vc120-debug.lib")
	#elif defined MSW_RUNTIME_RELEASE_SHARED
		#pragma comment(lib, "libzip-vc120-rt=shared.lib")
	#elif defined MSW_RUNTIME_RELEASE_STATIC
		#pragma comment(lib, "libzip-vc120.lib")
	#endif
#endif
