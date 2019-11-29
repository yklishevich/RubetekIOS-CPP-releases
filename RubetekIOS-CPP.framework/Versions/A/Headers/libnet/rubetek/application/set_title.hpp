#pragma once

#include <string>

#include <msw/config.hpp>
#include <msw/self/pid.hpp>
#include <msw/self/static_info.hpp>
#include <msw/console/set_typical_title.hpp>


namespace rubetek
{
    inline void set_title(std::string const& product, std::string const& optional = "")
    {
        msw::console::set_typical_title(product, optional);
    }
}
