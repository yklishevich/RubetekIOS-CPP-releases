#pragma once

#include <string>

#include <msw/config.hpp>
#include <msw/self/pid.hpp>
#include <msw/self/static_info.hpp>
#include <msw/console/set_title.hpp>


namespace msw     {
namespace console {

    inline void set_typical_title(std::string const& product, std::string const& optional = "")
    {
        msw::console::set_title(product + "[" +
#ifdef MSW_NO_COMPLETE_TO_STRING
            std::to_string(static_cast<long long int>(msw::self_pid()))
#else
            std::to_string(msw::self_pid())
#endif
            + "] " + msw::self_static_info::quick()
            + (!optional.empty() ? " | " : "")
            + optional
        );
    }

}}
