#pragma once

#include <string>

#include <msw/config.hpp>
#include <msw/has_boost_endian.hpp>

#ifdef MSW_HAS_BOOST_ENDIAN
#   include <boost/endian/conversion.hpp>
#endif
#ifdef MSW_NO_TO_STRING
#   include <boost/lexical_cast.hpp>
#endif

#include <msw/bitness.hpp>
#include <msw/native_bitness_64.hpp>


#ifdef BOOST_BIG_ENDIAN
#   define MSW_BIG_ENDIAN 1
#endif
#ifdef BOOST_LITTLE_ENDIAN
#   define MSW_LITTLE_ENDIAN 1
#endif


namespace msw
{
    struct self_static_info
    {
        static bool debug()
        {
#ifdef MSW_DEBUG
            return true;
#else
            return false;
#endif
        }
        static std::string const& architecture()
        {
            static std::string const arch =
#ifdef MSW_ARCHITECTURE_X86
                "x86"
#elif MSW_ARCHITECTURE_ARM
                "arm"
#elif MSW_ARCHITECTURE_POWER_PC
                "power-pc"
#elif MSW_ARCHITECTURE_ITANIUM
                "itanium"
#else
                ""
#endif
                ;
            return arch;
        }
        static std::string const& endian()
        {
            static std::string const s =
#ifdef MSW_BIG_ENDIAN
                "be"
#elif MSW_LITTLE_ENDIAN
                "le"
#else
                ""
#endif
                ;
            return s;
        }
        static std::string const& bitness()
        {
            static std::string const plfm =
#ifdef MSW_NO_COMPLETE_BOOL_TYPE
                std::is_same<std::true_type, bitness_x64>::value
#else
                bitness_64()
#endif
                    ? "64"
                    : (std::string("32") + (native_bitness_64() ? "(on-64)" : "")).c_str();
            return plfm;
        }
        static std::string const& compiler_name()
        {
            static std::string const compiler =
#ifdef MSW_MSVC
                "msvc"
#elif MSW_INTEL_CL
                "intel"
#elif MSW_CYGWIN
                "cygwin"
#elif MSW_MINGW
                "mingw"
#elif MSW_LLVM
                "llvm"
#elif MSW_CLANG
                "clang"
#elif MSW_GCC
                "gcc"
#else
                ""
#endif
                ;
            return compiler;
        }
        static std::string const& compiler_version()
        {
            static std::string const ver = make_version(
#ifdef MSW_MSVC
                _MSC_FULL_VER / 10000000, _MSC_FULL_VER / 100000 % 100, _MSC_FULL_VER % 100000
#elif MSW_INTEL_CL
                __INTEL_COMPILER / 100, __INTEL_COMPILER / 10 % 10, __INTEL_COMPILER % 10
#elif MSW_CYGWIN
                ""
#elif MSW_MINGW
                __MINGW32_MAJOR_VERSION, __MINGW32_MINOR_VERSION
#elif MSW_LLVM
                __clang_version__
#elif MSW_GCC
                __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__
#else
                ""
#endif
                )
#if defined(MSW_MINGW) || defined(MSW_LLVM)
                + " (gcc-" + make_version(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__) + ")"
#endif
            ;
            return ver;
        }
        static std::string const& compiler_info()
        {
            static std::string const info = compiler_name() + "-" + compiler_version();
            return info;
        }
        static std::string const& operation_system()
        {
            static std::string const os =
#ifdef MSW_WINDOWS
                "windows"
#elif MSW_APPLE
                "appale"
#elif MSW_ANDROID
                "android"
#elif MSW_LINUX
                "linux"
#else
                ""
#endif
                ;
            return os;
        }

        static std::string quick()
        {
            std::string endian_s;
            if (has_boost_endian) endian_s = "-" + endian();
            return make_total(architecture() + "-" + bitness() + endian_s, (debug() ? "debug" : ""));
        }
        static std::string mean()
        {
            return make_total(quick(), compiler_info());
        }
        static std::string total()
        {
            return make_total(mean(), operation_system());
        }

    private:

#ifdef MSW_NO_COMPLETE_TO_STRING
        static std::string make_version(int n)
        {
            return std::to_string(static_cast<long long int>(n));
        }
#endif
        template <typename N>
        static std::string make_version(N n)
        {
#ifdef MSW_NO_TO_STRING
            return boost::lexical_cast<std::string>(n);
#else
            return std::to_string(n);
#endif
        }
        template <typename N, typename ...Ns>
        static std::string make_version(N n, Ns... ns)
        {
            return make_version(n) + "." + make_version(ns...);
        }

        template <typename N>
        static std::string make_total(N n)
        {
            return n;
        }
        template <typename N, typename ...Ns>
        static std::string make_total(N n, Ns... ns)
        {
            std::string const left_part = make_total(n);
            std::string const right_part = make_total(ns...);
            if (left_part.empty()) return right_part;
            else if (right_part.empty()) return left_part;
            return left_part + " " + right_part;
        }
    };
}
