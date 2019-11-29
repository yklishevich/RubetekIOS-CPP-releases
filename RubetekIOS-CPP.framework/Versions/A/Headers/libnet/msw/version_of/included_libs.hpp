#pragma once

#ifdef BOOST_VERSION_HPP
#   define MSW_USING_BOOST 1
#endif
#ifdef QT_VERSION
#   define MSW_USING_QT 1
#endif
#ifdef FFMPEG_VERSION
#   define MSW_USING_FFMPEG 1
#endif
#ifdef PUGIXML_VERSION
#   define MSW_USING_PUGIXML 1
#endif
#ifdef LIBZIP_VERSION
#   define MSW_USING_LIBZIP 1
#endif
#ifdef ZLIB_VERSION
#   define MSW_USING_ZLIB 1
#endif
#ifdef _THRIFT_THRIFT_H_
#   define MSW_USING_THRIFT 1
#endif
#ifdef GUMBO_VERSION
#   define MSW_USING_GUMBO 1
#endif
#ifdef PCAP_VERSION_MAJOR
#   define MSW_USING_PCAP 1
#endif

#include <msw/version_of/msw.hpp>
#ifdef MSW_USING_BOOST
#   include <msw/version_of/boost.hpp>
#endif
#ifdef MSW_USING_QT
#   include <msw/version_of/qt.hpp>
#endif
#ifdef MSW_USING_FFMPEG
#   include <msw/version_of/ffmpeg.hpp>
#endif
#ifdef MSW_USING_PUGIXML
#   include <msw/version_of/pugixml.hpp>
#endif
#ifdef MSW_USING_LIBZIP
#   include <msw/version_of/libzip.hpp>
#endif
#ifdef MSW_USING_ZLIB
#   include <msw/version_of/zlib.hpp>
#endif
#ifdef MSW_USING_THRIFT
#   include <msw/version_of/thrift.hpp>
#endif
#ifdef MSW_USING_GUMBO
#   include <msw/version_of/gumbo.hpp>
#endif
#ifdef MSW_USING_PCAP
#   include <msw/version_of/pcap.hpp>
#endif


namespace msw
{
    inline std::string const& version_of_included_libs()
    {
        static std::string const v =
            "msw-" + version_of_msw()
#ifdef MSW_USING_BOOST
            + " boost-" + version_of_boost()
#endif
#ifdef MSW_USING_QT
            + " qt-" + version_of_qt()
#endif
#ifdef MSW_USING_FFMPEG
            + " ffmpeg-" + version_of_ffmpeg()
#endif
#ifdef MSW_USING_PUGIXML
            + " pugixml-" + version_of_pugixml()
#endif
#ifdef MSW_USING_LIBZIP
            + " libzip-" + version_of_libzip()
#endif
#ifdef MSW_USING_ZLIB
            + " zlib-" + version_of_zlib()
#endif
#ifdef MSW_USING_THRIFT
            + " thrift-" + version_of_thrift()
#endif
#ifdef MSW_USING_GUMBO
            + " gumbo-" + version_of_gumbo()
#endif
#ifdef MSW_USING_PCAP
            + " winpcap-" + version_of_pcap()
#endif
            ;
        return v;
    }
}

#undef MSW_USING_BOOST
#undef MSW_USING_QT
#undef MSW_USING_FFMPEG
#undef MSW_USING_PUGIXML
#undef MSW_USING_LIBZIP
#undef MSW_USING_ZLIB
#undef MSW_USING_THRIFT
#undef MSW_USING_GUMBO
#undef MSW_USING_PCAP
