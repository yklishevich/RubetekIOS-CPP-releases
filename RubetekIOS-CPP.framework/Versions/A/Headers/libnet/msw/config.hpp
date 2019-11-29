#pragma once


#ifdef NDEBUG
#   define MSW_RELEASE 1
#else
#   define MSW_DEBUG 1
#endif

#ifdef _MSC_VER
#   define MSW_MSVC 1
#   define MSW_WINDOWS 1
#endif

#ifdef __ICL
#   define MSW_INTEL_CL 1
#endif

#ifdef __CYGWIN__
#   define MSW_CYGWIN 1
#   define MSW_WINDOWS 1
#endif

#ifdef __MINGW32__
#   define MSW_MINGW 1
#   define MSW_WINDOWS 1
#endif

#ifdef __llvm__
#   define MSW_LLVM 1
#endif

#ifdef __clang__
#   define MSW_CLANG 1
#endif

#ifdef __GNUC__
#   define MSW_GCC 1
#endif

#ifdef __ANDROID__
#   define MSW_ANDROID 1
#endif

#ifdef __APPLE__
#   define MSW_APPLE 1
#endif

#ifndef MSW_WINDOWS
    #define MSW_LINUX 1
#endif

#if    (defined _WIN64        ) \
    || (defined _M_X64        ) \
    || (defined _M_AMD64      ) \
    || (defined __amd64__     ) \
    || (defined __amd64       ) \
    || (defined __x86_64__    ) \
    || (defined __x86_64      ) \
    || (defined __LP64__      ) \
    || (defined __aarch64__   ) \
    || (defined __powerpc64__ ) \
    || (defined __ppc64__     ) \
    || (defined __PPC64__     )
#   define MSW_BITNESS_64 1
#else
#   define MSW_BITNESS_32 1
#endif

#if    (defined i386       ) /*gnu*/            \
    || (defined __i386     )                    \
    || (defined __i386__   )                    \
    || (defined __IA32__   ) /*Stratus VOS*/    \
    || (defined _M_IX86    ) /* msvc, intel-c*/ \
    || (defined _X86__     ) /*Watcom*/         \
    || (defined _X86_      ) /*mingw*/          \
    || (defined _WIN64     )                    \
    || (defined __amd64__  )                    \
    || (defined __amd64    )                    \
    || (defined __x86_64__ )                    \
    || (defined __x86_64   )
#   define MSW_ARCHITECTURE_X86 1
#endif

#if    (defined __arm__     ) /*gnu*/        \
    || (defined __aarch64__ )                \
    || (defined _M_ARM      ) /*msvc*/       \
    || (defined _ARM        ) /*ImageCraft*/ \
    || (defined _arm        ) /*Diab*/
#   define MSW_ARCHITECTURE_ARM 1
#endif

#if    (defined __powerpc     ) /*gnu*/ \
    || (defined __powerpc__   )         \
    || (defined __powerpc64__ )         \
    || (defined __POWERPC__   )         \
    || (defined __ppc__       )         \
    || (defined __ppc64__     )         \
    || (defined __PPC__       )         \
    || (defined __PPC64__     )         \
    || (defined _ARCH_PPC     )         \
    || (defined _ARCH_PPC64   )         \
    || (defined _M_PPC        ) /*msvc*/
#   define MSW_ARCHITECTURE_POWER_PC 1
#endif


#if    (defined __ia64__    ) /*gnu*/            \
    || (defined _IA64       )                    \
    || (defined __IA64__    )                    \
    || (defined __ia64      ) /*HP aCC*/         \
    || (defined _M_IA64     ) /* msvc, intel-c*/ \
    || (defined __itanium__ ) /*intel-c*/
#   define MSW_ARCHITECTURE_ITANIUM 1
#endif

#ifdef MSW_MSVC
    #ifdef _MT
        #ifdef _DEBUG
            #ifndef MSW_DEBUG
                #error "Mismatch: project-release and runtime-debug"
            #endif
            #ifdef _DLL
                #define MSW_RUNTIME_DEBUG_SHARED 1
            #else
                #define MSW_RUNTIME_DEBUG_STATIC 1
            #endif
        #else
            #ifndef MSW_RELEASE
                #error "Mismatch: project-debug and runtime-release"
            #endif
            #ifdef _DLL
                #define MSW_RUNTIME_RELEASE_SHARED 1
            #else
                #define MSW_RUNTIME_RELEASE_STATIC 1
            #endif
        #endif
    #else
        #error "Compiler threading support is not turned on. Please set the correct command line options for threading: either /MT /MTd /MD or /MDd"
    #endif
#endif

#ifdef MSW_DISABLE_LIBS_AUTOLINK
#   define MSW_DISABLE_LIBZIP_AUTOLINK 1
#   define MSW_DISABLE_THRIFT_AUTOLINK 1
#   define MSW_DISABLE_OPENSSL_AUTOLINK 1
#   define MSW_DISABLE_PCAP_AUTOLINK 1
#endif

#ifdef MSW_GCC
#   define MSW_BOOST_REGEX 1
#   define MSW_MAKE_UNIQUE 1
#endif

#ifdef MSW_MINGW
#   define MSW_BOOST_MUTEX 1
#   define MSW_BOOST_THREAD 1
#   define MSW_BOOST_CHRONO 1
#   define MSW_BOOST_CONDITION_VARIABLE 1
#endif

#ifdef MSW_CLANG
#   define MSW_NO_TO_STRING 1
#endif

#ifdef MSW_WINDOWS
#   define MSW_WINAPI 1
#   define MSW_WINAPI_SYSTEM_ERROR 1
#   define MSW_WINAPI_FILE 1
#   define MSW_WINAPI_DLL 1
#   define MSW_WINAPI_SELF_EXE_NAME 1
#   define MSW_WINAPI_SELF_PID 1
#   define MSW_WINAPI_EXECUTE_PROCESS 1
#endif

#ifdef MSW_WINAPI
#   define MSW_ON_WINAPI(...) __VA_ARGS__
#else
#   define MSW_ON_WINAPI(...)
#endif

#ifdef MSW_LINUX
#   define MSW_ON_LINUX(...) __VA_ARGS__
#else
#   define MSW_ON_LINUX(...)
#endif

#ifdef MSW_GCC_4_4
#   define MSW_NO_FOR 1
#   define MSW_NO_LAMBDA 1
#   define MSW_NO_NULLPTR 1
#   define MSW_NO_RESULT_OF 1
#   define MSW_BOOST_THREAD 1
#   define MSW_BOOST_CHRONO 1
#   define MSW_NO_MOVE_IN_MAP 1
#   define MSW_ODD_STD_FORWARD 1
#   define MSW_NO_COMPLETE_STRING 1
#   define MSW_NO_FORWARD_AS_TUPLE 1
#   define MSW_NO_TEMPLATE_ID_EXPR 1
#   define MSW_NO_COMPLETE_TO_STRING 1
#   define MSW_NO_COMPLETE_BOOL_TYPE 1
#   define MSW_NO_COMPLETE_TEMPLATES 1
#   define MSW_NO_UNORDERED_CONTAINERS 1
#   define MSW_NO_DELEGATING_CONSTRUCTORS 1
#   define MSW_NO_ENUM_OPERATOR_COMPARISON 1
#   define MSW_NO_EXPLICIT_CONVERSION_OPERATORS 1
#endif

#ifdef MSW_NO_NULLPTR
#   define nullptr 0
#endif

#ifndef MSW_NO_THRIFT_VERSION_HPP
#   define MSW_THRIFT_VERSION_HPP 1
#endif

#ifndef MSW_DISABLE_WINMAIN
#   define MSW_WINMAIN
#endif
