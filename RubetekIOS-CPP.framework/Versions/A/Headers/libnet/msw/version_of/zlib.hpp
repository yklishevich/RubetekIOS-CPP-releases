#pragma once

#ifndef ZLIB_VERSION
#   include <zlib.h>
#endif


namespace msw
{
    static char const* version_of_zlib()
    {
        return ZLIB_VERSION;
    }
}
