#pragma once

#include <functional>

#include <boost/system/error_code.hpp>


namespace msw
{
    typedef std::function<void(boost::system::error_code const&)> on_error;
}
