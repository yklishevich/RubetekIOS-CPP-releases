#pragma once

#include <msw/handle.hpp>


namespace msw
{
    struct process_info
    {
        msw::handle::native_type handle ;
        int32                    pid    ;
    };
}
