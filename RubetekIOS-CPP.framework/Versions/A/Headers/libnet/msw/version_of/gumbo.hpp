#pragma once

#ifndef GUMBO_VERSION
#   include <gumbo_version.h>
#endif


namespace msw
{
    static char const* version_of_gumbo()
    {
        return GUMBO_VERSION;
    }
}
