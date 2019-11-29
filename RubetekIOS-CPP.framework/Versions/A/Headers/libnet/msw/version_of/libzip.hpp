#pragma once

#ifndef LIBZIP_VERSION
#   include <zipconf.h>
#endif


namespace msw
{
    static char const* version_of_libzip()
    {
        return LIBZIP_VERSION;
    }
}
