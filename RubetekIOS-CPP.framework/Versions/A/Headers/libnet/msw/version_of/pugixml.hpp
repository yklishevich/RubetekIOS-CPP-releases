#pragma once

#include <string>

#ifndef PUGIXML_VERSION
#   include <pugixml/pugixml.hpp>
#endif

#include <msw/config.hpp>


namespace msw
{
    static std::string const& version_of_pugixml()
    {
        static std::string const ver =
            std::to_string
#ifdef MSW_NO_COMPLETE_TO_STRING
                (static_cast<long long int>
#endif
                    (PUGIXML_VERSION / 100)
#ifdef MSW_NO_COMPLETE_TO_STRING
                )
#endif
            + "." +
            std::to_string
#ifdef MSW_NO_COMPLETE_TO_STRING
                (static_cast<long long int>
#endif
                    (PUGIXML_VERSION % 100)
#ifdef MSW_NO_COMPLETE_TO_STRING
                )
#endif
            ;
        return ver;
    }
}
