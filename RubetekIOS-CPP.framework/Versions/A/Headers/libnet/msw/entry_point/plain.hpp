#pragma once

#include <msw/entry_point/entry_point.hpp>


namespace msw
{
    void main();
}

MSW_ENTRY_POINT
(
      msw::entry_point::type::plain
    , msw::main
)
