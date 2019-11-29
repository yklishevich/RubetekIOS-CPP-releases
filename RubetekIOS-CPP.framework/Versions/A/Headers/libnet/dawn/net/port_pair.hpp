#pragma once

#include <msw/plain_types.hpp>


namespace dawn {
namespace net  {

    struct port_pair
    {
        msw::wbyte source      = 0;
        msw::wbyte destination = 0;

        bool any_equal(msw::wbyte port) const
        {
            return source == port || destination == port;
        }
    };

}}
