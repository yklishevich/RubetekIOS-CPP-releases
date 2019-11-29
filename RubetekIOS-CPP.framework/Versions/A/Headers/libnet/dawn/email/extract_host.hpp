#pragma once

#include <string>

#include <msw/std/regex.hpp>
#include <msw/throw_runtime_error.hpp>


namespace dawn
{
    inline std::string email_extract_host(std::string const& address)
    {
        msw::smatch what;
        static msw::regex e("@(.*)");
        if (msw::regex_search(address, what, e)) return what[1];
        msw::throw_runtime_error("extract host fail, email address not correct");
#include <msw/throw_hide_warning.hpp>
    }
}
