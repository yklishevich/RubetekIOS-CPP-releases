#pragma once


#ifdef BOOST_VERSION
#   if (BOOST_VERSION / 100 % 1000) > 58
#       define MSW_HAS_BOOST_ENDIAN 1
#   endif
#endif


namespace msw
{
    static bool const has_boost_endian =
#ifdef MSW_HAS_BOOST_ENDIAN
    true
#else
    false
#endif
    ;
}
