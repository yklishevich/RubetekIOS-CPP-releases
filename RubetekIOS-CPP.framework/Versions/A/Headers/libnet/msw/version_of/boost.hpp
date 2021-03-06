#pragma once

#include <string>

#include <boost/version.hpp>

#include <msw/config.hpp>


namespace msw
{
    static std::string const& version_of_boost()
    {
        static std::string const ver =
            std::to_string
#ifdef MSW_NO_COMPLETE_TO_STRING
                (static_cast<long long int>
#endif
                    (BOOST_VERSION / 100000)
#ifdef MSW_NO_COMPLETE_TO_STRING
                )
#endif
            + "."
            + std::to_string
#ifdef MSW_NO_COMPLETE_TO_STRING
                (static_cast<long long int>
#endif
                    (BOOST_VERSION / 100 % 1000)
#ifdef MSW_NO_COMPLETE_TO_STRING
                )
#endif
            + "."
            + std::to_string
#ifdef MSW_NO_COMPLETE_TO_STRING
                (static_cast<long long int>
#endif
                    (BOOST_VERSION % 100)
#ifdef MSW_NO_COMPLETE_TO_STRING
                )
#endif
            ;
        return ver;
    }
}
