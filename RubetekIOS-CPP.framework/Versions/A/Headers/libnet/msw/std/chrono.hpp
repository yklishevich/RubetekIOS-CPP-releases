#pragma once

#include <msw/config.hpp>

#ifdef MSW_BOOST_CHRONO
#   include <boost/chrono.hpp>
#else
#   include <chrono>
#endif


namespace msw
{
#ifdef MSW_BOOST_CHRONO
    namespace chrono = boost::chrono;
#else
    namespace chrono = std::chrono;
#endif
}
