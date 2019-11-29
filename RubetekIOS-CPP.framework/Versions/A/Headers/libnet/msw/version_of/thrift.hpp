#pragma once

#ifdef MSW_THRIFT_VERSION_HPP
//#   include <thrift/version.hpp>
#else
#   ifndef THRIFT_VERSION
#       define THRIFT_VERSION "?"
#   endif
#endif


namespace msw
{
    static char const* version_of_thrift()
    {
        return "?";//THRIFT_VERSION;
    }
}
