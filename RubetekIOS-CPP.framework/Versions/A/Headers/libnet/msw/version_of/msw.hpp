#pragma once

#include <msw/version.hpp>

#include <msw/config.hpp>


namespace msw
{
    static std::string const& version_of_msw()
    {
        static std::string const ver =
            std::to_string
#ifdef MSW_NO_COMPLETE_TO_STRING
                (static_cast<long long int>
#endif
                    (MSW_VERSION / 100)
#ifdef MSW_NO_COMPLETE_TO_STRING
                )
#endif
                + "."
            + std::to_string
#ifdef MSW_NO_COMPLETE_TO_STRING
                (static_cast<long long int>
#endif
                    (MSW_VERSION / 10 % 100)
#ifdef MSW_NO_COMPLETE_TO_STRING
                )
#endif
                + "."
            + std::to_string
#ifdef MSW_NO_COMPLETE_TO_STRING
                (static_cast<long long int>
#endif
                    (MSW_VERSION % 10)
#ifdef MSW_NO_COMPLETE_TO_STRING
                )
#endif
            ;
        return ver;
    }
}
