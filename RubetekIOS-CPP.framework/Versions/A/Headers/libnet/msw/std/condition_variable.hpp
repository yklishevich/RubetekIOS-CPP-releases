#pragma once

#include <msw/config.hpp>

#ifdef MSW_BOOST_CONDITION_VARIABLE
#   include <msw/include/boost/thread/condition_variable.hpp>
#else
#   include <condition_variable>
#endif


namespace msw
{
#ifdef MSW_BOOST_CONDITION_VARIABLE
    using boost::condition_variable;
#else
    using std::condition_variable;
#endif
}
