#pragma once

#include <msw/config.hpp>

#ifdef MSW_BOOST_MUTEX
#   include <msw/include/boost/thread/mutex.hpp>
#   include <msw/include/boost/thread/recursive_mutex.hpp>
#else
#   include <mutex>
#endif


namespace msw
{
#ifdef MSW_BOOST_MUTEX
    using boost::mutex;
    using boost::recursive_mutex;
    using boost::unique_lock;
#else
    using std::mutex;
    using std::recursive_mutex;
    using std::unique_lock;
#endif
}
