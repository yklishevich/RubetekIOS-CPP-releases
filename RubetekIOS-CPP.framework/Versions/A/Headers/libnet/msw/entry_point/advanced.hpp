#pragma once

#include <msw/entry_point/entry_point.hpp>


namespace msw
{
    void main(command_line);
}


MSW_ENTRY_POINT
(
      msw::entry_point::type::advanced
    , msw::main
)
