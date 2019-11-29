#pragma once

#include <msw/config.hpp>

#ifdef MSW_BOOST_THREAD
#   include <msw/include/boost/thread.hpp>
#else
#   include <thread>
#endif


namespace msw
{
#ifdef MSW_BOOST_THREAD
    using boost::thread;
    namespace this_thread = boost::this_thread;
#else
    using std::thread;
    namespace this_thread = std::this_thread;
#endif
}
